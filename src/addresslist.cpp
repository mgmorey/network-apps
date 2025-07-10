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

#include "network/addresslist.hpp"      // AddressList
#include "network/format-ai-error.hpp"  // format_ai_error()
#include "network/hostnameview.hpp"     // HostnameView
#include "network/optionalhints.hpp"    // OptionalHints
#include "network/oserror.hpp"          // OsError
#include "network/serviceview.hpp"      // ServiceView
#include "network/sharedruntime.hpp"    // SharedRuntime
#include "network/stream-addrinfo.hpp"  // operator<<()
#include "network/string-null.hpp"      // string_null
#include "network/to-os-error.hpp"      // to_os_error()

#ifdef _WIN32
#include <ws2tcpip.h>       // addrinfo, ::freeaddrinfo(), ::getaddrinfo()
#else
#include <netdb.h>          // addrinfo, ::freeaddrinfo(), ::getaddrinfo()
#endif

#include <iostream>     // std::cout, std::endl
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

auto Network::AddressList::InputIterator::operator==(const InputIterator& t_rhs)
    const noexcept -> bool
{
    return m_node == t_rhs.m_node;
}

Network::AddressList::StringOrNull::StringOrNull(std::string_view t_sv)
{
    if (t_sv.data() != nullptr) {
        m_value = t_sv;
    }
}

auto Network::AddressList::StringOrNull::c_str() const -> const char*
{
    return m_value ? m_value->c_str() : nullptr;
}

Network::AddressList::AddressList(const HostnameView& t_hostname,
                                  const ServiceView& t_service,
                                  const OptionalHints& t_hints,
                                  const SharedRuntime& t_sr)
{
    const auto hints_ptr {to_ai_ptr(t_hints)};
    const auto hints_str {to_string(t_hints)};
    const StringOrNull hostname {t_hostname};
    const StringOrNull service {t_service};

    if (t_sr->is_verbose()) {
        // clang-format off
        std::cout << "Calling ::getaddrinfo("
                  << hostname
                  << ", "
                  << service
                  << ", "
                  << hints_str
                  << ", ...)"
                  << std::endl;
        // clang-format on
    }

    if (const auto error {::getaddrinfo(hostname.c_str(),
                                        service.c_str(),
                                        hints_ptr.get(),
                                        &m_list)}) {
        const auto os_error {to_os_error(error)};
        std::ostringstream oss;
        // clang-format off
        oss << "Call to ::getaddrinfo("
            << hostname
            << ", "
            << service
            << ", "
            << hints_str
            << ", ...) returned "
            << error
            << ": "
            << format_ai_error(error);
        // clang-format on
        m_result = {os_error, oss.str()};
    }
    else if (t_sr->is_verbose()) {
        for (const auto& node : *this) {
            std::cout << node << std::endl;
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

auto Network::AddressList::result() const noexcept -> const OsError&
{
    return m_result;
}

auto Network::AddressList::to_ai_ptr(const OptionalHints& t_hints) noexcept ->
    std::unique_ptr<addrinfo>
{
    return t_hints ? std::make_unique<addrinfo>(*t_hints) : nullptr;
}

auto Network::AddressList::to_string(const OptionalHints& t_hints) ->
    std::string
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

auto Network::operator<<(std::ostream& os,
                         const AddressList::StringOrNull& str) -> std::ostream&
{
    return os << str.m_value.value_or(string_null);
}
