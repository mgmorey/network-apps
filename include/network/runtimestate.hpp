// Copyright (C) 2025  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef UNIX_NETWORK_RUNTIMESTATE_HPP
#define UNIX_NETWORK_RUNTIMESTATE_HPP

#ifndef WIN32

#include "network/failmode.hpp"         // FailMode

namespace Network
{
    struct RuntimeState
    {
        int m_error_code {0};
        FailMode m_fail_mode {FailMode::throw_error};
        bool m_is_started {false};
        bool m_is_verbose {false};
    };
}

#endif

#endif
