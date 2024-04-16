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

#include "network/get-name.h"           // get_name()
#include "network/bytestring.h"         // ByteString
#include "network/error.h"              // Error
#include "network/get-nameresult.h"     // get_nameresult()
#include "network/getnamehandler.h"     // GetNameHandler
#include "network/getnameparams.h"      // GetNameParams
#include "network/oserrorresult.h"      // OsErrorResult
#include "network/overloaded.h"         // Overloaded

#include <variant>      // std::visit()

auto Network::get_name(const GetNameHandler& handler,
                       const GetNameParams& args) -> ByteString
{
    ByteString result;
    auto name_result {get_nameresult(handler, args)};
    std::visit(Overloaded {
            [&](const ByteString& addr) {
                result = addr;
            },
                [&](const OsErrorResult& error) {
                    throw Error(error.string());
                }
                }, name_result);
    return result;
}
