// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_START_RUNTIME_HPP
#define NETWORK_START_RUNTIME_HPP

#include "network/failmode.hpp"         // FailMode
#include "network/sharedruntime.hpp"    // SharedRuntime
#ifdef WIN32
#include "network/version.hpp"          // Version
#endif

namespace Network
{
#ifdef WIN32
    extern auto start_runtime(Version t_version,
                              FailMode t_fail_mode,
                              bool t_is_verbose) -> SharedRuntime;
#endif
    extern auto start_runtime(FailMode t_fail_mode,
                              bool t_is_verbose) -> SharedRuntime;
    extern auto start_runtime(bool t_is_verbose = false) -> SharedRuntime;
}

#endif
