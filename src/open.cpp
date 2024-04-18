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
#include "network/create-socketresult.h"        // create_socketresult()
#include "network/error-strings.h"              // VISITOR_ERROR
#include "network/open-socket.h"                // open()
#include "network/openendpointparams.h"         // OpenEndpointParams
#include "network/openhandler.h"                // OpenHandler
#include "network/openresult.h"                 // OpenResult
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/socket.h"                     // Socket
#include "network/template.h"                   // Template

#include <iostream>     // std::endl
#include <type_traits>  // std::decay_t, std::is_same_v
#include <variant>      // std::visit()

Network::Open::Open(const OpenHandler& t_handler,
                    const OpenEndpointParams& t_args,
                    std::ostream& t_os) :
    m_handler(t_handler),
    m_args(t_args),
    m_os(t_os)
{
}

auto Network::Open::operator()(const Template& t_temp) -> SocketResult
{
    if (m_args.verbose) {
        m_os << t_temp
             << std::endl;
    }

    auto template_result {create_socketresult(t_temp.hints(), m_args.verbose)};
    std::visit([&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, Socket>) {
            const auto result {open(m_handler, {arg,
                                                t_temp.address(),
                                                m_args.verbose})};

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
            static_assert(always_false_v<T>, VISITOR_ERROR);
        }
    }, template_result);
    return template_result;
}
