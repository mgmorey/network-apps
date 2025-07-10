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

#include "network/bind-endpoint.hpp"            // bind()
#include "network/endpointview.hpp"             // EndpointView
#include "network/open-endpoint.hpp"            // open()
#include "network/openinputs.hpp"               // OpenInputs
#include "network/opensymbol.hpp"               // OpenSymbol
#include "network/run.hpp"                      // run()
#include "network/sharedruntime.hpp"            // SharedRuntime
#include "network/sockethints.hpp"              // SocketHints
#include "network/socketresultvector.hpp"       // SocketResultVector

auto Network::bind(const EndpointView& endpoint,
                   const SocketHints& hints,
                   const SharedRuntime& sr) -> SocketResultVector
{
    const OpenInputs oi {endpoint, hints, sr};
    return open(oi, OpenSymbol::bind);
}

auto Network::bind(const EndpointView& endpoint,
                   const SocketHints& hints,
                   bool is_verbose) -> SocketResultVector
{
    return bind(endpoint, hints, run(is_verbose));
}
