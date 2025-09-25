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
#include "network/bytespan.hpp"                 // ByteSpan
#include "network/endpoint.hpp"                 // Endpoint
#include "network/error.hpp"                    // Error
#include "network/get-endpointresult.hpp"       // get_endpointresult()
#include "network/run.hpp"                      // run()
#include "network/runtime.hpp"                  // Runtime

auto Network::get_endpoint(ByteSpan bs,
                           int flags,
                           const Runtime& rt) -> Endpoint
{
    const auto result {get_endpointresult(bs, flags, rt)};

    if (!result) {
        throw Error {result.error().string()};
    }

    return *result;
}

auto Network::get_endpoint(ByteSpan bs,
                           int flags,
                           bool is_verbose) -> Endpoint
{
    const auto sr {run(is_verbose)};
    return get_endpoint(bs, flags, *sr);
}
