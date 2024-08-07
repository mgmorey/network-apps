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

#ifndef NETWORK_OPEN_TEMPLATES_H
#define NETWORK_OPEN_TEMPLATES_H

#include "network/openendpointparams.h"         // OpenEndpointParams
#include "network/openhandler.h"                // OpenHandler
#include "network/socketresultvector.h"         // SocketResultVector
#include "network/templatevector.h"             // TemplateVectorResult

namespace Network
{
    extern auto open(const OpenHandler& handler,
                     const OpenEndpointParams& args,
                     const TemplateVector& templates) -> SocketResultVector;
}

#endif
