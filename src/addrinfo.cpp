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

#include "network/addrinfo.h"           // AddrInfo, OptionalHints,
                                        // OptionalHostname,
                                        // OptionalService,
                                        // OsErrorResult, addrinfo,
                                        // operator<<()
#include "network/format-ai-error.h"    // format_ai_error()
#include "network/string-null.h"        // string_null

#ifdef WIN32
#include <ws2tcpip.h>   // freeaddrinfo(), getaddrinfo()
#else
#include <netdb.h>      // freeaddrinfo(), getaddrinfo()
#endif

#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream
#include <string>       // std::string

Network::AddrInfo::InputIterator::InputIterator(pointer t_pointer) noexcept :
    m_list(t_pointer)
{
}

auto Network::AddrInfo::InputIterator::operator*() const noexcept ->
    Network::AddrInfo::InputIterator::reference
{
    return *m_list;
}

auto Network::AddrInfo::InputIterator::operator->() const noexcept ->
    Network::AddrInfo::InputIterator::pointer
{
    return m_list;
}

auto Network::AddrInfo::InputIterator::operator++() noexcept ->
    Network::AddrInfo::InputIterator&
{
    m_list = m_list->ai_next;
    return *this;
}

auto Network::AddrInfo::InputIterator::operator==(const InputIterator& rhs)
    const noexcept -> bool
{
    return m_list == rhs.m_list;
}

auto Network::AddrInfo::end() noexcept -> Network::AddrInfo::InputIterator
{
    return nullptr;
}

auto Network::AddrInfo::format(const std::unique_ptr<addrinfo>& hints) ->
    std::string
{
    std::ostringstream oss;

    if (hints) {
        oss << *hints;
    }
    else {
        oss << string_null;
    }

    return oss.str();
}

Network::AddrInfo::AddrInfo(const OptionalHostname& t_hostname,
                            const OptionalService& t_service,
                            const OptionalHints& t_hints,
                            bool t_verbose)
{
    const std::unique_ptr<addrinfo> hints = t_hints ?
        std::make_unique<addrinfo>(*t_hints) :
        nullptr;

    if (t_verbose) {
        std::cout << "Calling ::getaddrinfo("
                  << t_hostname.value_or(string_null)
                  << ", "
                  << t_service.value_or(string_null)
                  << ", "
                  << format(hints)
                  << ", ...)"
                  << std::endl;
    }

    if (const auto error {::getaddrinfo(to_c_string(t_hostname),
                                        to_c_string(t_service),
                                        hints.get(),
                                        &m_list)}) {
        std::ostringstream oss;
        oss << "Call to ::getaddrinfo("
            << t_hostname.value_or(string_null)
            << ", "
            << t_service.value_or(string_null)
            << ", "
            << format(hints)
            << ", ...) returned "
            << error
            << ": "
            << format_ai_error(error);
        m_result = {error, oss.str()};
    }
}

Network::AddrInfo::~AddrInfo() noexcept
{
    if (m_list != nullptr) {
        ::freeaddrinfo(m_list);
    }
}

auto Network::AddrInfo::begin() const noexcept ->
    Network::AddrInfo::InputIterator
{
    return m_list;
}

auto Network::AddrInfo::result() const -> Network::OsErrorResult
{
    return m_result;
}

auto Network::AddrInfo::to_c_string(const OptionalString& str) noexcept ->
    const char*
{
    return str ? str->c_str() : nullptr;
}
