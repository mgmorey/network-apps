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
#include "network/optionalhints.h"      // OptionalHints
#include "network/optionalhostname.h"   // OptionalHostname
#include "network/optionalservice.h"    // OptionalService
#include "network/oserrorresult.h"      // OsErrorResult
#include "network/string-null.h"        // string_null
#include "network/to-os-error.h"        // to_os_error()

#ifdef WIN32
#include <ws2tcpip.h>   // addrinfo, ::freeaddrinfo(), ::getaddrinfo()
#else
#include <netdb.h>      // addrinfo, ::freeaddrinfo(), ::getaddrinfo()
#endif

#include <iostream>     // std::cerr, std::cout, std::endl
#include <memory>       // std::make_unique, std::unique_ptr
#include <optional>     // std::optional
#include <sstream>      // std::ostringstream
#include <string>       // std::string

Network::AddressList::InputIterator::InputIterator(pointer t_pointer) noexcept :
    m_list(t_pointer)
{
}

auto Network::AddressList::InputIterator::operator*() const noexcept ->
    InputIterator::reference
{
    return *m_list;
}

auto Network::AddressList::InputIterator::operator->() const noexcept ->
    InputIterator::pointer
{
    return m_list;
}

auto Network::AddressList::InputIterator::operator++() noexcept ->
    InputIterator&
{
    m_list = m_list->ai_next;
    return *this;
}

auto Network::AddressList::InputIterator::operator==(const InputIterator& rhs)
    const noexcept -> bool
{
    return m_list == rhs.m_list;
}

auto Network::AddressList::format(const std::unique_ptr<addrinfo>& hints) ->
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

Network::AddressList::AddressList(const OptionalHostname& t_hostname,
                                  const OptionalService& t_service,
                                  const OptionalHints& t_hints,
                                  bool t_verbose)
{
    const auto hints {t_hints ? std::make_unique<addrinfo>(*t_hints) : nullptr};

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

    if (const auto error {::getaddrinfo(to_c_str(t_hostname),
                                        to_c_str(t_service),
                                        hints.get(),
                                        &m_list)}) {
        const auto os_error {to_os_error(error)};
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
        m_result = {os_error, oss.str()};
    }
    else if (t_verbose) {
        std::cerr << *this;
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

auto Network::AddressList::to_c_str(const std::optional<std::string>& str)
    noexcept -> const char*
{
    return str ? str->c_str() : nullptr;
}
