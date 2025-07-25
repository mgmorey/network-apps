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
#include "network/apioptions.hpp"       // ApiOptions
#include "network/get-runtime.hpp"      // get_runtime()
#include "network/runtimescope.hpp"     // RuntimeScope
#include "network/sharedruntime.hpp"    // SharedRuntime

auto Network::run(ApiOptions ao, RuntimeScope rs) -> SharedRuntime
{
    auto sr {get_runtime(ao, rs)};
    sr->start();
    return sr;
}

auto Network::run(RuntimeScope rs, bool is_verbose) -> SharedRuntime
{
    return run(ApiOptions {is_verbose}, rs);
}

auto Network::run(bool is_verbose) -> SharedRuntime
{
    return run(RuntimeScope::global, is_verbose);
}
