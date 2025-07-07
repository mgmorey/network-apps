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
#include "network/sockettemplate.hpp"           // SocketTemplate
#include "network/sockettemplatevector.hpp"     // SocketTemplateVector

#include <algorithm>    // std::ranges::transform()
#include <iterator>     // std::back_inserter()

auto Network::open(const OpenParameters& op, bool is_bind) -> SocketResultVector
{
    SocketTemplateVector stv;

    if (const auto error = insert(std::back_inserter(stv), op)) {
        throw Error(error.string());
    }

    SocketResultVector srv;
    auto open = [&](const SocketTemplate& st) -> SocketResult {
        auto result {create_socketresult(st.hints(), op.m_sr)};

        if (result) {
            const auto& bs {st.address()};
            const auto& ps {*result};

            if (const auto error {ps->open(bs, is_bind)}) {
                return std::unexpected {error};
            }
        }

        return result;
    };
    std::ranges::transform(stv, std::back_inserter(srv), open);
    return srv;
}
