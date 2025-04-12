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

#ifndef WIN32

#include "network/contextdata.hpp"      // ContextData
#include "network/optionalversion.hpp"  // OptionalVersion
#include "network/start.hpp"            // start()

#include <iostream>     // std::cout, std::endl

static constexpr auto system_description {
    "Berkeley Software Distribution Sockets"
};
static constexpr auto system_running {
    "Running"
};

auto Network::start(const OptionalVersion& version,
                    bool is_verbose) -> ContextData
{
    if (is_verbose) {
        std::cout << "Starting the network runtime."
                  << std::endl;
    }

    static_cast<void>(version);
    return {.m_description = system_description,
            .m_system_status = system_running};
}

#endif
