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
#include "network/bytespan.hpp"                 // ByteSpan
#include "network/endpoint.hpp"                 // Endpoint
#include "network/error-strings.hpp"            // VISITOR_ERROR
#include "network/error.hpp"                    // Error
#include "network/get-endpointresult.hpp"       // get_endpointresult()
#include "network/oserrorresult.hpp"            // OsErrorResult,
#include "network/run.hpp"                      // run()
#include "network/sharedruntime.hpp"            // SharedRuntime

#include <type_traits>  // std::decay_t, std::is_same_v
#include <variant>      // std::visit()

auto Network::get_endpoint(ByteSpan bs,
                           int flags,
                           const SharedRuntime& sr) -> Endpoint
{
    Endpoint result;
    auto endpoint_result {get_endpointresult(bs, flags, sr->is_verbose())};
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

auto Network::get_endpoint(ByteSpan bs,
                           int flags,
                           bool is_verbose) -> Endpoint
{
    const auto rt {run(is_verbose)};
    return get_endpoint(bs, flags, rt);
}
