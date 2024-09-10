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

#include "network/open.h"                       // Open
#include "network/always-false.h"               // always_false_v
#include "network/create-result.h"              // create_result()
#include "network/error-strings.h"              // VISITOR_ERROR
#include "network/openendpointparams.h"         // OpenEndpointParams
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/socketresult.h"               // SocketResult
#include "network/template.h"                   // Template
#include "network/uniquesocket.h"               // UniqueSocket

#include <iostream>     // std::cout, std::endl
#include <type_traits>  // std::decay_t, std::is_same_v
#include <variant>      // std::visit()

Network::Open::Open(const OpenEndpointParams& t_args, bool t_is_bind)
    : m_args(t_args),
      m_is_bind(t_is_bind)
{
}

auto Network::Open::operator()(const Template& t_temp) -> SocketResult
{
    if (m_args.is_verbose) {
        std::cout << t_temp
                  << std::endl;
    }

    auto result {create_result(t_temp.hints(), m_args.is_verbose)};
    std::visit([&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, UniqueSocket>) {
            if (const auto error_result {arg->open(t_temp.address(), m_is_bind)}) {
                result = error_result;
            }
        }
        else if constexpr (std::is_same_v<T, OsErrorResult>) {
            static_cast<void>(arg);
        }
        else {
            static_assert(always_false_v<T>, VISITOR_ERROR);
        }
    }, result);
    return result;
}
