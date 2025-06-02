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

#include "network/run.hpp"              // run()
#include "network/create-runtime.hpp"   // create_runtime()
#include "network/get-runtime.hpp"      // get_runtime()
#include "network/logicerror.hpp"       // LogicError
#include "network/runtimedata.hpp"      // RuntimeData
#include "network/runtimescope.hpp"     // RuntimeScope
#include "network/sharedruntime.hpp"    // SharedRuntime

auto Network::run(RuntimeData rd, RuntimeScope scope) -> SharedRuntime
{
    SharedRuntime sr;

    switch (scope) {
    case RuntimeScope::global:
        sr = get_runtime(rd);
        break;
    case RuntimeScope::shared:
        sr = create_runtime(rd);
        break;
    default:
        throw LogicError("Invalid scope");
    }

    sr->start();
    return sr;
}

auto Network::run(RuntimeScope scope, bool is_verbose) -> SharedRuntime
{
    return run(RuntimeData {is_verbose}, scope);
}

auto Network::run(bool is_verbose) -> SharedRuntime
{
    return run(RuntimeScope::global, is_verbose);
}
