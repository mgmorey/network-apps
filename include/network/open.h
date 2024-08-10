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

#ifndef NETWORK_OPEN_H
#define NETWORK_OPEN_H

#include "network/openendpointparams.h"         // OpenEndpointParams
#include "network/openhandler.h"                // OpenHandler
#include "network/socketresult.h"               // SocketResult
#include "network/template.h"                   // Template

#include <iostream>     // std::cout, std::endl

namespace Network
{
    class Open
    {
    public:
        Open(const OpenHandler& t_handler,
             const OpenEndpointParams& t_args,
             std::ostream& t_os);
        auto operator()(const Template& t_temp) -> SocketResult;

    private:
        const OpenHandler& m_handler;
        const OpenEndpointParams& m_args;
        std::ostream& m_os;
    };
}

#endif
