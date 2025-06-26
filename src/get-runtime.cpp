// Copyright (C) 2025  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/get-runtime.hpp"      // get-runtime()
#include "network/apioptions.hpp"       // ApiOptions
#include "network/create-runtime.hpp"   // create_runtime()
#include "network/logicerror.hpp"       // LogicError
#include "network/runtimescope.hpp"     // RuntimeScope
#include "network/sharedruntime.hpp"    // SharedRuntime

auto Network::get_runtime(ApiOptions ao) -> SharedRuntime
{
    static auto sr {create_runtime(ao)};
    return sr;
}

auto Network::get_runtime(ApiOptions ao, RuntimeScope rs) -> SharedRuntime
{
    switch (rs) {
    case RuntimeScope::global:
        return get_runtime(ao);
    case RuntimeScope::shared:
        return create_runtime(ao);
    default:
        throw LogicError("Invalid scope");
    }
}
