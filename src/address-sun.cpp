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

#include "network/address.h"            // Address, std::string
#include "network/get-sun-path.h"       // get_sun_path()
#include "network/os-features.h"        // WIN32
#include "network/string-null.h"        // string_null

#ifndef WIN32

auto Network::Address::sun_text() const noexcept -> std::string
{
    return get_sun_path(m_value).value_or(string_null);
}

#endif
