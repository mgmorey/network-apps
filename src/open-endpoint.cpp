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
#include "network/overloaded.h"                 // Overloaded
#include "network/socket.h"                     // Socket
#include "network/socketresultvector.h"         // SocketResultVector
#include "network/template.h"                   // Template
#include "network/templatevector.h"             // TemplateVector

#include <algorithm>    // std::transform()
#include <iostream>     // std::cout, std::endl
#include <iterator>     // std::back_inserter()
#include <variant>      // std::visit()

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
        std::visit(Overloaded {
                [&](const Socket& sock) {
                    const auto result {open(handler, {sock,
                                                      temp.address(),
                                                      args.verbose})};

                    if (result) {
                        template_result = result;
                    }
                    else {
                        template_result = sock;
                    }
                },
                    [&](const OsErrorResult& result) {
                        static_cast<void>(result);
                    }
                    }, template_result);
        return template_result;
    };
    const auto templates_result {get_templates(args.endpoint,
                                               args.hints,
                                               args.verbose)};
    std::visit(Overloaded {
            [&](const TemplateVector& templates) {
                std::transform(templates.begin(), templates.end(),
                               std::back_inserter(socket_results),
                               lambda);
                open_result = OpenResult {socket_results};
            },
                [&](const OsErrorResult& result) {
                    open_result = OpenResult {result};
                }
                }, templates_result);
    return open_result;
}
