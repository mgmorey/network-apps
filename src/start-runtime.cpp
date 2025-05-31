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

#include "network/start-runtime.hpp"    // start_runtime()
#include "network/failmode.hpp"         // FailMode
#include "network/sharedruntime.hpp"    // SharedRuntime
#include "network/socketapi.hpp"        // SocketApi
#ifdef WIN32
#include "network/version.hpp"          // Version
#endif

#include <memory>       // std::make_unique()
#include <utility>      // std::move()

namespace {
    auto start(std::unique_ptr<Network::Runtime> unique) ->
        Network::SharedRuntime
    {
        static Network::SharedRuntime shared {std::move(unique)};

        if (shared) {
            shared->start();
        }

        return shared;
    }
}

#ifdef WIN32

auto Network::start_runtime(Version t_version,
                            FailMode t_fail_mode,
                            bool t_is_verbose) -> SharedRuntime
{
    return ::start(std::make_unique<Network::SocketApi>(t_version,
                                                        t_fail_mode,
                                                        t_is_verbose));
}

#endif

auto Network::start_runtime(FailMode t_fail_mode,
                            bool t_is_verbose) -> SharedRuntime
{
    return ::start(std::make_unique<Network::SocketApi>(t_fail_mode,
                                                        t_is_verbose));
}

auto Network::start_runtime(bool t_is_verbose) -> SharedRuntime
{
    return ::start(std::make_unique<Network::SocketApi>(t_is_verbose));
}
