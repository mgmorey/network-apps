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

#include "network/get-endpoint.h"       // Endpoint, OsErrorResult,
                                        // get_endpoint()
#include "network/error.h"              // Error
#include "network/get-endpointresult.h" // EndpointResult,
                                        // get_endpointresult()
#include "network/overloaded.h"         // Overloaded


auto Network::get_endpoint(const ByteString& addr, int flags, bool verbose) ->
    Network::Endpoint
{
    Endpoint result;
    const auto endpoint_result {get_endpointresult(addr, flags, verbose)};
    std::visit(Overloaded {
            [&](const Endpoint& endpoint) {
                result = endpoint;
            },
            [&](const OsErrorResult& error) {
                throw Error(error.string());
            }
        }, endpoint_result);
    return result;
}
