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

#include "network/get-hostname.hpp"             // get_hostname()
#include "network/always-false.hpp"             // always_false_v
#include "network/error-strings.hpp"            // VISITOR_ERROR
#include "network/error.hpp"                    // Error
#include "network/get-hostnameresult.hpp"       // get_hostname()
#include "network/hostname.hpp"                 // Hostname
#include "network/oserrorresult.hpp"            // OsErrorResult
#include "network/run.hpp"                      // run()
#include "network/sharedruntime.hpp"            // SharedRuntime

#include <type_traits>  // std::decay_t, std::is_same_v
#include <variant>      // std::visit()

auto Network::get_hostname(const SharedRuntime& sr) -> Hostname
{
    Hostname result;
    auto hostname_result {get_hostnameresult(sr)};
    std::visit([&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, Hostname>) {
            result = arg;
        }
        else if constexpr (std::is_same_v<T, OsErrorResult>) {
            throw Error(arg.string());
        }
        else {
            static_assert(always_false_v<T>, VISITOR_ERROR);
        }
    }, hostname_result);
    return result;
}

auto Network::get_hostname(bool is_verbose) -> Hostname
{
    const auto rt {run(is_verbose)};
    return get_hostname(rt);
}
