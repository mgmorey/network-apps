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

#include "network/open-endpoint.h"              // open()
#include "network/create-socketresult.h"        // create_socketresult()
#include "network/get-templates-endpoint.h"     // get_templates()
#include "network/open-socket.h"                // open()
#include "network/openendpointparams.h"         // OpenEndpointParams
#include "network/openhandler.h"                // OpenHandler
#include "network/openresult.h"                 // OpenResult
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/socket.h"                     // Socket
#include "network/socketresultvector.h"         // SocketResultVector
#include "network/template.h"                   // Template
#include "network/templatevector.h"             // TemplateVector

#include <algorithm>    // std::transform()
#include <iostream>     // std::cout, std::endl
#include <iterator>     // std::back_inserter()
#include <type_traits>  // std::decay_t, std::is_same_v
#include <variant>      // std::visit()

template <class>
inline constexpr bool always_false_v {false};

auto Network::open(const OpenHandler& handler,
                   const OpenEndpointParams& args) -> OpenResult
{
    OpenResult open_result;
    SocketResultVector socket_results;
    const auto lambda = [&](const Template& temp) {
        if (args.verbose) {
            std::cout << temp
                      << std::endl;
        }

        auto template_result {create_socketresult(temp.hints(), args.verbose)};
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, Socket>) {
                const auto result {open(handler, {arg,
                                                  temp.address(),
                                                  args.verbose})};

                if (result) {
                    template_result = result;
                }
                else {
                    template_result = arg;
                }
            }
            else if constexpr (std::is_same_v<T, OsErrorResult>) {
                static_cast<void>(arg);
            }
            else {
                static_assert(always_false_v<T>, "non-exhaustive visitor!");
            }
        }, template_result);
        return template_result;
    };
    const auto templates_result {get_templates(args.endpoint,
                                               args.hints,
                                               args.verbose)};
    std::visit([&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, TemplateVector>) {
            std::transform(arg.begin(), arg.end(),
                           std::back_inserter(socket_results),
                           lambda);
            open_result = socket_results;
        }
        else if constexpr (std::is_same_v<T, OsErrorResult>) {
            open_result = arg;
        }
        else {
            static_assert(always_false_v<T>, "non-exhaustive visitor!");
        }
    }, templates_result);
    return open_result;
}
