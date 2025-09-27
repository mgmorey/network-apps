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

#ifndef _WIN32

#include "network/create-socketpair.hpp"        // create_socketpair()
#include "network/create-socketpairresult.hpp"  // create_socketpairresult()
#include "network/error.hpp"                    // Error
#include "network/run.hpp"                      // run()
#include "network/runtime.hpp"                  // Runtime
#include "network/sockethints.hpp"              // SocketHints
#include "network/socketpair.hpp"               // SocketPair

#include <utility>      // std::move()

auto Network::create_socketpair(const SocketHints& hints,
                                const Runtime* rt) -> SocketPair
{
    auto result {create_socketpairresult(hints, rt)};

    if (!result) {
        throw Error {result.error().string()};
    }

    return std::move(*result);
}

auto Network::create_socketpair(const SocketHints& hints,
                                bool is_verbose) -> SocketPair
{
    const auto sr {run(is_verbose)};
    return create_socketpair(hints, sr.get());
}

#endif
