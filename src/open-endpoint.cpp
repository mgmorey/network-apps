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
#include "network/always-false.h"               // always_false_v
#include "network/error-strings.h"              // VISITOR_ERROR
#include "network/get-templates-endpoint.h"     // get_templates()
#include "network/open-templates.h"             // open()
#include "network/openhandler.h"                // OpenHandler
#include "network/openparams.h"                 // OpenParams
#include "network/openresult.h"                 // OpenResult
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/templatevector.h"             // TemplateVector

#include <type_traits>  // std::decay_t, std::is_same_v
#include <variant>      // std::visit()

auto Network::open(const OpenHandler& handler,
                   const OpenParams& args) -> OpenResult
{
    OpenResult open_result;
    const auto templates_result {get_templates(args)};
    std::visit([&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, TemplateVector>) {
            open_result = open(handler, args, arg);
        }
        else if constexpr (std::is_same_v<T, OsErrorResult>) {
            open_result = arg;
        }
        else {
            static_assert(always_false_v<T>, VISITOR_ERROR);
        }
    }, templates_result);
    return open_result;
}
