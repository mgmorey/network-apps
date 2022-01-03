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

#include "network/get-sa-data.h"        // Bytes, get_sa_data()
#include "network/get-sa-family.h"      // get_sa_family()
#include "network/sa-offsets.h"         // sa_data_offset

auto Network::get_sa_data(const Bytes& addr) -> Network::Bytes
{
    assert(get_sa_family(addr) != 0);  // NOLINT
    return addr.substr(sa_data_offset);
}
