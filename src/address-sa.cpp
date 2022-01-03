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

#include "network/address.h"            // Address, family_type,
                                        // length_type, std::string,
                                        // value_type
#include "network/get-sa-family.h"      // get_sa_family()
#include "network/get-sa-length.h"      // get_sa_length()
#include "network/get-sa-pointer.h"     // get_sa_pointer()
#include "network/offsets.h"            // sa_data_offset
#include "network/to-string.h"          // to_string()

auto Network::Address::sa() const -> const sockaddr*
{
    return get_sa_pointer(m_value);
}

auto Network::Address::sa_data() const -> Network::Address::value_type
{
    return m_value.substr(sa_data_offset);
}

auto Network::Address::sa_family() const -> Network::family_type
{
    return get_sa_family(m_value);
}

auto Network::Address::sa_length() const -> Network::Address::length_type
{
    return get_sa_length(m_value);
}

auto Network::Address::sa_text() const -> std::string
{
    return to_string(m_value);
}
