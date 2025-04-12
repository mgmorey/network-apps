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

#include "network/get-endpoint.hpp"             // get_endpoint()
#include "network/always-false.hpp"             // always_false_v
#include "network/endpoint.hpp"                 // Endpoint
#include "network/error-strings.hpp"            // VISITOR_ERROR
#include "network/error.hpp"                    // Error
#include "network/get-endpointresult.hpp"       // get_endpointresult()
#include "network/oserrorresult.hpp"            // OsErrorResult,

#include <cstddef>      // std::byte
#include <span>         // std::span
#include <type_traits>  // std::decay_t, std::is_same_v
#include <variant>      // std::visit()

auto Network::get_endpoint(std::span<const std::byte> bs,
                           int flags,
                           bool is_verbose) -> Endpoint
{
    Endpoint result;
    auto endpoint_result {get_endpointresult(bs, flags, is_verbose)};
    std::visit([&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, Endpoint>) {
            result = arg;
        }
        else if constexpr (std::is_same_v<T, OsErrorResult>) {
            throw Error(arg.string());
        }
        else {
            static_assert(always_false_v<T>, VISITOR_ERROR);
        }
    }, endpoint_result);
    return result;
}
