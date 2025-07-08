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

#include "network/get-name.hpp"         // get_name()
#include "network/bytestring.hpp"       // ByteString
#include "network/error.hpp"            // Error
#include "network/get-nameresult.hpp"   // get_nameresult()
#include "network/namehandler.hpp"      // NameHandler
#include "network/socketcore.hpp"       // SocketCore

auto Network::get_name(const SocketCore& sc,
                       const NameHandler& nh) -> ByteString
{
    auto result {get_nameresult(sc, nh)};

    if (!result) {
        throw Error(result.error().string());
    }

    return *result;
}
