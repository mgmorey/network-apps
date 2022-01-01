// Copyright (C) 2022  "Michael G. Morey" <mgmorey@gmail.com>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "network/addrinfo.h"       // AddrInfo, Hints, Hostname,
                                    // Result, Service, addrinfo,
                                    // operator<<()

#ifdef _WIN32
#include <ws2tcpip.h>   // freeaddrinfo(), getaddrinfo()
#else
#include <netdb.h>      // freeaddrinfo(), getaddrinfo()
#endif

#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream
#include <string>       // std::string

static auto get_hints(addrinfo* info, const Network::Hints* hints) -> addrinfo*
{
    if (hints == nullptr) {
        info = nullptr;
    }
    else {
        *info = *hints;
    }

    return info;
}

Network::AddrInfo::InputIterator::InputIterator(pointer t_pointer) :
    m_pointer(t_pointer)
{
}

auto Network::AddrInfo::InputIterator::operator*() const ->
    Network::AddrInfo::InputIterator::reference
{
    return *m_pointer;
}

auto Network::AddrInfo::InputIterator::operator->() const ->
    Network::AddrInfo::InputIterator::pointer
{
    return m_pointer;
}

auto Network::AddrInfo::InputIterator::operator++() ->
    Network::AddrInfo::InputIterator&
{
    m_pointer = m_pointer->ai_next;
    return *this;
}

auto Network::AddrInfo::List::end() -> Network::AddrInfo::InputIterator
{
    return nullptr;
}

Network::AddrInfo::List::List(const Hostname& t_node,
                              const Service& t_serv,
                              const Hints* t_hints,
                              bool t_verbose)
{
    if (t_verbose) {
        std::cerr << "Trying socket hints:"
                  << std::endl
                  << *t_hints
                  << std::endl;
    }

    addrinfo ai = Network::Hints();
    auto *const hints {::get_hints(&ai, t_hints)};

    if (t_verbose) {
        std::cerr << "Calling getaddrinfo("
                  << t_node
                  << ", "
                  << t_serv
                  << ", ...)"
                  << std::endl;
    }

    if (const auto error = ::getaddrinfo(t_node, t_serv, hints, &m_pointer)) {
        std::ostringstream oss;
        oss << "Call to getaddrinfo("
            << t_node
            << ", "
            << t_serv
            << ", ...) returned "
            << error
            << ": "
            << ::gai_strerror(error);
        m_result = {error, oss.str()};
    }
}

Network::AddrInfo::List::~List()
{
    if (m_pointer != nullptr) {
        ::freeaddrinfo(m_pointer);
    }
}

auto Network::AddrInfo::List::begin() const -> Network::AddrInfo::InputIterator
{
    return m_pointer;
}

auto Network::AddrInfo::List::result() const -> Network::Result
{
    return m_result;
}

auto Network::AddrInfo::operator==(const InputIterator& left,
                                   const InputIterator& right) -> bool
{
    return left.m_pointer == right.m_pointer;
}

auto Network::AddrInfo::operator!=(const InputIterator& left,
                                   const InputIterator& right) -> bool
{
    return left.m_pointer != right.m_pointer;
}
