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

#include "network/addresslist.h"        // AddressList
#include "network/format-ai-error.h"    // format_ai_error()
#include "network/hostnameview.h"       // HostnameView
#include "network/optionalhints.h"      // OptionalHints
#include "network/oserrorresult.h"      // OsErrorResult
#include "network/serviceview.h"        // ServiceView
#include "network/stream-addrinfo.h"    // operator<<()
#include "network/string-null.h"        // string_null
#include "network/to-os-error.h"        // to_os_error()

#ifdef WIN32
#include <ws2tcpip.h>       // addrinfo, ::freeaddrinfo(), ::getaddrinfo()
#else
#include <netdb.h>          // addrinfo, ::freeaddrinfo(), ::getaddrinfo()
#endif

#include <iostream>     // std::cerr, std::cout, std::endl
#include <memory>       // std::make_unique, std::unique_ptr
#include <sstream>      // std::ostringstream
#include <string>       // std::string

Network::AddressList::InputIterator::InputIterator(pointer t_pointer) noexcept :
    m_node(t_pointer)
{
}

auto Network::AddressList::InputIterator::operator*() const noexcept ->
    InputIterator::reference
{
    return *m_node;
}

auto Network::AddressList::InputIterator::operator->() const noexcept ->
    InputIterator::pointer
{
    return m_node;
}

auto Network::AddressList::InputIterator::operator++() noexcept ->
    InputIterator&
{
    m_node = m_node->ai_next;
    return *this;
}

auto Network::AddressList::InputIterator::operator==(const InputIterator& rhs)
    const noexcept -> bool
{
    return m_node == rhs.m_node;
}

Network::AddressList::AddressList(const HostnameView& t_hostname,
                                  const ServiceView& t_service,
                                  const OptionalHints& t_hints,
                                  bool t_is_verbose)
{
    const auto hints_ptr {to_ai_ptr(t_hints)};
    const auto hints_str {format(t_hints)};

    if (t_is_verbose) {
        std::cout << "Calling ::getaddrinfo("
                  << (t_hostname.data() == nullptr ? string_null : t_hostname)
                  << ", "
                  << (t_service.data() == nullptr ? string_null : t_service)
                  << ", "
                  << hints_str
                  << ", ...)"
                  << std::endl;
    }

    if (const auto error {::getaddrinfo(t_hostname.data(),
                                        t_service.data(),
                                        hints_ptr.get(),
                                        &m_list)}) {
        const auto os_error {to_os_error(error)};
        std::ostringstream oss;
        oss << "Call to ::getaddrinfo("
            << (t_hostname.data() == nullptr ? string_null : t_hostname)
            << ", "
            << (t_service.data() == nullptr ? string_null : t_service)
            << ", "
            << hints_str
            << ", ...) returned "
            << error
            << ": "
            << format_ai_error(error);
        m_result = {os_error, oss.str()};
    }
    else if (t_is_verbose) {
        for (const auto& node : *this) {
            std::cerr << node << std::endl;
        }
    }
}

Network::AddressList::~AddressList() noexcept
{
    if (m_list != nullptr) {
        ::freeaddrinfo(m_list);
    }
}

auto Network::AddressList::begin() const noexcept -> InputIterator
{
    return m_list;
}

auto Network::AddressList::end() const noexcept -> InputIterator // NOLINT
{
    return nullptr;
}

auto Network::AddressList::result() const noexcept -> const OsErrorResult&
{
    return m_result;
}

auto Network::AddressList::format(const OptionalHints& t_hints) -> std::string
{
    std::ostringstream oss;

    if (t_hints) {
        oss << static_cast<addrinfo>(*t_hints);
    }
    else {
        oss << string_null;
    }

    return oss.str();
}

auto Network::AddressList::to_ai_ptr(const OptionalHints& t_hints) noexcept ->
    std::unique_ptr<addrinfo>
{
    return t_hints ? std::make_unique<addrinfo>(*t_hints) : nullptr;
}
