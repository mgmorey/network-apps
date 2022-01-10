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

#include "network/addrinfo.h"           // AddrInfo, Hints, Hostname,
                                        // Result, Service, addrinfo,
                                        // operator<<()

#ifdef _WIN32
#include <ws2tcpip.h>   // freeaddrinfo(), getaddrinfo()
#else
#include <netdb.h>      // freeaddrinfo(), getaddrinfo()
#endif

#include <iostream>     // std::cerr, std::endl
#include <memory>       // std::unique_ptr
#include <sstream>      // std::ostringstream
#include <string>       // std::string

Network::AddrInfo::InputIterator::InputIterator(pointer t_pointer) :
    m_list(t_pointer)
{
}

auto Network::AddrInfo::InputIterator::operator*() const ->
    Network::AddrInfo::InputIterator::reference
{
    return *m_list;
}

auto Network::AddrInfo::InputIterator::operator->() const ->
    Network::AddrInfo::InputIterator::pointer
{
    return m_list;
}

auto Network::AddrInfo::InputIterator::operator++() ->
    Network::AddrInfo::InputIterator&
{
    m_list = m_list->ai_next;
    return *this;
}

auto Network::AddrInfo::List::end() -> Network::AddrInfo::InputIterator
{
    return nullptr;
}

Network::AddrInfo::List::List(const Hostname& t_node,
                              const Service& t_service,
                              const OptionalHints& t_hints,
                              bool t_verbose)
{
    std::unique_ptr<addrinfo> hints = t_hints ?
        std::make_unique<addrinfo>(*t_hints) :
        nullptr;

    if (t_verbose) {
        if (t_hints) {
        std::cerr << "Trying socket hints:"
                  << std::endl
                  << *t_hints
                  << std::endl;
        }
    }

    if (t_verbose) {
        std::cerr << "Calling getaddrinfo("
                  << t_node
                  << ", "
                  << t_service
                  << ", ...)"
                  << std::endl;
    }

    if (const auto error = ::getaddrinfo(static_cast<const char*>(t_node),
                                         static_cast<const char*>(t_service),
                                         hints.get(),
                                         &m_list)) {
        std::ostringstream oss;
        oss << "Call to getaddrinfo("
            << t_node
            << ", "
            << t_service
            << ", ...) returned "
            << error
            << ": "
            << ::gai_strerror(error);
        m_result = {error, oss.str()};
    }
}

Network::AddrInfo::List::~List()
{
    if (m_list != nullptr) {
        ::freeaddrinfo(m_list);
    }
}

auto Network::AddrInfo::List::begin() const -> Network::AddrInfo::InputIterator
{
    return m_list;
}

auto Network::AddrInfo::List::result() const -> Network::Result
{
    return m_result;
}

auto Network::AddrInfo::operator==(const InputIterator& left,
                                   const InputIterator& right) -> bool
{
    return left.m_list == right.m_list;
}

auto Network::AddrInfo::operator!=(const InputIterator& left,
                                   const InputIterator& right) -> bool
{
    return left.m_list != right.m_list;
}
