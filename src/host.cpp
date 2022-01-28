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

#include "network/host.h"               // ByteString, Host,
                                        // OptionalHostname, addrinfo
#include "network/to-byte-string-sa.h"  // to_byte_string()

Network::Host::Host(const addrinfo& t_addrinfo) noexcept :
    m_addr(to_byte_string(t_addrinfo.ai_addr, t_addrinfo.ai_addrlen)),
    m_name(to_canonical_name(t_addrinfo.ai_canonname))
{
}

auto Network::Host::operator=(const addrinfo& t_addrinfo) noexcept ->
    Network::Host&
{
    m_addr = to_byte_string(t_addrinfo.ai_addr, t_addrinfo.ai_addrlen);
    m_name = to_canonical_name(t_addrinfo.ai_canonname);
    return *this;
}

auto Network::Host::operator<(const Host& t_host) const noexcept -> bool
{
    return m_addr < t_host.m_addr;	// NOLINT
}

auto Network::Host::operator>(const Host& t_host) const noexcept -> bool
{
    return m_addr > t_host.m_addr;	// NOLINT
}

auto Network::Host::operator==(const Host& t_host) const noexcept -> bool
{
    return m_addr == t_host.m_addr;	// NOLINT
}

auto Network::Host::address() const noexcept -> Network::ByteString
{
    return m_addr;
}

auto Network::Host::canonical_name() const noexcept -> Network::OptionalHostname
{
    return m_name;
}

auto Network::Host::to_canonical_name(const char* t_str) noexcept ->
    Network::OptionalHostname
{
    if (t_str == nullptr) {
        return {};
    }

    return {t_str};
}
