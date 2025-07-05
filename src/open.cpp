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

#include "network/open.hpp"                     // Open
#include "network/create-socketresult.hpp"      // create_socketresult()
#include "network/openparameters.hpp"           // OpenParameters
#include "network/socketresult.hpp"             // SocketResult
#include "network/template.hpp"                 // Template

#include <expected>     // std::unexpected

Network::Open::Open(const OpenParameters& t_op, bool t_is_bind) :
    m_sr(t_op.m_sr),
    m_is_bind(t_is_bind)
{
}

auto Network::Open::operator()(const Template& t_template) const -> SocketResult
{
    auto result {create_socketresult(t_template.hints(), m_sr)};

    if (result) {
        const auto& bs {t_template.address()};
        const auto& ps {*result};

        if (const auto error {m_is_bind ? ps->bind(bs) : ps->connect(bs)}) {
            return std::unexpected {error};
        }
    }

    return result;
}
