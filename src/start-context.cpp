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

#include "network/start-context.hpp"    // start_context()
#include "network/failuremode.hpp"      // FailureMode
#include "network/sockets.hpp"          // Sockets
#include "network/uniquecontext.hpp"    // UniqueContext
#ifdef WIN32
#include "network/version.hpp"          // Version
#endif

#include <memory>       // std::make_unique()

#ifdef WIN32

auto Network::start_context(Version t_version,
                            FailureMode t_failure,
                            bool t_is_verbose) -> UniqueContext
{
    auto context {std::make_unique<Network::Sockets>(t_version,
                                                     t_failure,
                                                     t_is_verbose)};

    if (context) {
        context->start();
    }

    return context;
}

#endif

auto Network::start_context(FailureMode t_failure,
                            bool t_is_verbose) -> UniqueContext
{
    auto context {std::make_unique<Network::Sockets>(t_failure,
                                                     t_is_verbose)};

    if (context) {
        context->start();
    }

    return context;
}

auto Network::start_context(bool t_is_verbose) -> UniqueContext
{
    auto context {std::make_unique<Network::Sockets>(t_is_verbose)};

    if (context) {
        context->start();
    }

    return context;
}
