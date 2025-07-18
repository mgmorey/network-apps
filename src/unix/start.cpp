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

#ifndef _WIN32

#include "network/start.hpp"            // start()
#include "network/apioptions.hpp"       // ApiOptions
#include "network/apistate.hpp"         // ApiState

#include <iostream>     // std::cout, std::endl

auto Network::start(ApiOptions ao) -> ApiState
{
    if (ao.is_verbose()) {
        std::cout << "Starting the network runtime."
                  << std::endl;
    }

    ApiState as;
    as.m_version = Version {0, 0};
    as.m_high_version = Version {0, 0},
    as.m_description = "Berkeley Software Distribution Sockets",
    as.m_system_status = "Running";
    return as;
}

#endif
