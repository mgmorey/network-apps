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
#include "network/create-socketresult.hpp"      // create_socketresult()
#include "network/error.hpp"                    // Error()
#include "network/insert-endpoint.hpp"          // insert()
#include "network/openparameters.hpp"           // OpenParameters
#include "network/socketresult.hpp"             // SocketResult
#include "network/socketresultvector.hpp"       // SocketResultVector
#include "network/template.hpp"                 // Template

#include <algorithm>    // std::ranges::transform()
#include <iterator>     // std::back_inserter()
#include <vector>       // std::vector

auto Network::open(const OpenParameters& op, bool is_bind) -> SocketResultVector
{
    std::vector<Template> tv;
    auto it {std::back_inserter(tv)};

    if (const auto os_error = insert(it, op)) {
        throw Error(os_error.string());
    }

    SocketResultVector srv;
    auto open = [&](const Template& t) -> SocketResult {
        auto result {create_socketresult(t.hints(), op.m_sr)};

        if (result) {
            const auto& bs {t.address()};
            const auto& ps {*result};

            if (const auto error {is_bind ? ps->bind(bs) : ps->connect(bs)}) {
                return std::unexpected {error};
            }
        }

        return result;
    };
    std::ranges::transform(tv, std::back_inserter(srv), open);
    return srv;
}
