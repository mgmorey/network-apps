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

#ifndef NETWORK_CONTEXT_H
#define NETWORK_CONTEXT_H

#include "network/oserrorresult.h"        // OsErrorResult

#ifdef _WIN32
#include <winsock2.h>       // MAKEWORD(), WORD, WSADATA
#endif

#include <ostream>      // std::ostream

namespace Network
{
    class Context
    {
    public:
        Context() = delete;
        Context(const Context&) = delete;
        Context(const Context&&) = delete;
        explicit Context(bool t_verbose = false,
                         WORD t_version = m_version);
        ~Context();
        auto operator=(const Context&) -> Context& = delete;
        auto operator=(const Context&&) -> Context& = delete;
        [[nodiscard]] auto result() const -> OsErrorResult;

    protected:
#ifdef _WIN32
        static constexpr WORD m_version {MAKEWORD(2, 2)};
        static unsigned m_count;
        static WSADATA m_data;
#endif

    private:
        OsErrorResult m_result {};
    };

    extern auto operator<<(std::ostream& os,
                           const Context& context) -> std::ostream&;
}

#endif
