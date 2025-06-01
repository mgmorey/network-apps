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

#include "network/start-runtime.hpp"    // start()
#include "network/runtimedata.hpp"      // RuntimeData
#include "network/sharedruntime.hpp"    // SharedRuntime
#include "network/socketapi.hpp"        // SocketApi

#include <memory>       // std::make_shared()

auto Network::start(const RuntimeData& rd) -> SharedRuntime
{
    static auto sr {std::make_shared<Network::SocketApi>(rd)};
    sr->start();
    return sr;
}
