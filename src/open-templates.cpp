// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/open-templates.h"             // open()
#include "network/open.h"                       // Open
#include "network/openendpointparams.h"         // OpenEndpointParams
#include "network/openhandler.h"                // OpenHandler
#include "network/socketresultvector.h"         // SocketResultVector
#include "network/templatevector.h"             // TemplateVector

#include <algorithm>    // std::transform()
#include <iostream>     // std::cout
#include <iterator>     // std::back_inserter()

auto Network::open(const OpenHandler& handler,
                   const OpenEndpointParams& args,
                   const TemplateVector& templates) -> SocketResultVector

{
    SocketResultVector socket_results;
    std::transform(templates.begin(), templates.end(),
                   std::back_inserter(socket_results),
                   Open(handler, args, std::cout));
    return socket_results;
}
