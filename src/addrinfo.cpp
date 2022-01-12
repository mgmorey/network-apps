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

#include "network/addrinfo.h"           // AddrInfo, ErrorResult,
                                        // List, OptionalHints,
                                        // OptionalHostname,
                                        // OptionalService, addrinfo,
                                        // operator<<()
#include "network/string-null.h"        // string_null

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

Network::AddrInfo::List::List(const OptionalHostname& t_hostname,
                              const OptionalService& t_service,
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
                  << t_hostname.value_or(string_null)
                  << ", "
                  << t_service.value_or(string_null)
                  << ", ...)"
                  << std::endl;
    }

    if (const auto error = ::getaddrinfo(to_c_string(t_hostname),
                                         to_c_string(t_service),
                                         hints.get(),
                                         &m_list)) {
        std::ostringstream oss;
        oss << "Call to getaddrinfo("
            << t_hostname.value_or(string_null)
            << ", "
            << t_service.value_or(string_null)
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

auto Network::AddrInfo::List::end() -> Network::AddrInfo::InputIterator
{
    return nullptr;
}

auto Network::AddrInfo::List::result() const -> Network::ErrorResult
{
    return m_result;
}

auto Network::AddrInfo::List::to_c_string(const OptionalString& str) ->
    const char*
{
    return str ? str->c_str() : nullptr;
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
