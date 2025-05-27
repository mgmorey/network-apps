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
#include "network/version.hpp"          // Version

#include <iostream>     // std::cout, std::endl

namespace {
    constexpr auto system_description {
        "Berkeley Software Distribution Sockets"
    };
    constexpr auto system_running {
        "Running"
    };

    auto start([[maybe_unused]] Network::OptionalVersion version,
               bool is_verbose) -> Network::ContextData
    {
        if (is_verbose) {
            std::cout << "Starting the network runtime."
                      << std::endl;
        }

        return {.m_description = system_description,
                .m_system_status = system_running};
    }
} // namespace

auto Network::start([[maybe_unused]] Version version,
                    bool is_verbose) -> ContextData
{
    return ::start(version, is_verbose);
}

auto Network::start(bool is_verbose) -> ContextData
{
    return ::start({}, is_verbose);
}

#endif
