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

#ifndef NETWORK_INSERT_ENDPOINT_H
#define NETWORK_INSERT_ENDPOINT_H

#include "network/insert-hostname.h"            // insert()
#include "network/openendpointparams.h"         // OpenEndpointParams
#include "network/oserrorresult.h"              // OsErrorResult

namespace Network
{
    auto insert(auto& it, const OpenEndpointParams& args) -> OsErrorResult
    {
        return insert(it,
                      args.m_endpoint.at(0),
                      args.m_endpoint.at(1),
                      args.m_hints,
                      args.m_is_verbose);
    }
}

#endif
