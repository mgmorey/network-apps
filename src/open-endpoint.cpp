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

#include "network/open-endpoint.hpp"            // open()
#include "network/error.hpp"                    // Error()
#include "network/insert-endpoint.hpp"          // insert()
#include "network/open.hpp"                     // Open
#include "network/openparameters.hpp"           // OpenParameters
#include "network/socketresultvector.hpp"       // SocketResultVector
#include "network/template.hpp"                 // Template

#include <algorithm>    // std::ranges
#include <iterator>     // std::back_inserter()
#include <vector>       // std::vector

auto Network::open(const OpenParameters& args, bool is_bind) -> SocketResultVector
{
    std::vector<Template> templates;
    auto it {std::back_inserter(templates)};

    if (const auto error_result = insert(it, args)) {
        throw Error(error_result.string());
    }

    SocketResultVector socket_results;
    std::ranges::transform(templates,
                           std::back_inserter(socket_results),
                           Open(args, is_bind));
    return socket_results;
}
