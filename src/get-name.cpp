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
#include "network/always-false.hpp"     // always_false_v
#include "network/bytestring.hpp"       // ByteString
#include "network/error-strings.hpp"    // VISITOR_ERROR
#include "network/error.hpp"            // Error
#include "network/get-nameresult.hpp"   // get_nameresult()
#include "network/oserrorresult.hpp"    // OsErrorResult
#include "network/socketdata.hpp"       // SocketData

#include <type_traits>  // std::decay_t, std::is_same_v
#include <variant>      // std::visit()

auto Network::get_name(const SocketData& sd, bool is_sockname) -> ByteString
{
    ByteString result;
    auto name_result {get_nameresult(sd, is_sockname)};
    std::visit([&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, ByteString>) {
            result = arg;
        }
        else if constexpr (std::is_same_v<T, OsErrorResult>) {
            throw Error(arg.string());
        }
        else {
            static_assert(always_false_v<T>, VISITOR_ERROR);
        }
    }, name_result);
    return result;
}
