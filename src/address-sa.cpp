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

#include "network/address.h"                    // Address
#include "network/get-sa-family.h"              // get_sa_family()
#include "network/os-features.h"                // HAVE_SOCKADDR_SA_LEN
#include "network/socket-family-type.h"         // socket_family_type
#include "network/to-string-vector-byte.h"      // to_string()

#ifdef HAVE_SOCKADDR_SA_LEN
#include "network/get-sa-length.h"              // get_sa_length()
#include "network/socket-length-type.h"         // socket_length_type
#endif

#include <string>       // std::string

auto Network::Address::sa_family() const -> socket_family_type
{
    return get_sa_family(m_value);
}

#ifdef HAVE_SOCKADDR_SA_LEN

auto Network::Address::sa_length() const -> socket_length_type
{
    return get_sa_length(m_value);
}

#endif

auto Network::Address::sa_text() const -> std::string
{
    return to_string(m_value);
}
