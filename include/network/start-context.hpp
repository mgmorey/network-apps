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

#ifndef NETWORK_START_CONTEXT_HPP
#define NETWORK_START_CONTEXT_HPP

#include "network/failuremode.hpp"      // FailureMode
#include "network/uniquecontext.hpp"    // UniqueContext
#include "network/version.hpp"          // Version

namespace Network
{
    extern auto start_context(Version t_version,
                              FailureMode t_failure,
                              bool t_is_verbose) -> UniqueContext;
    extern auto start_context(FailureMode t_failure,
                              bool t_is_verbose) -> UniqueContext;
    extern auto start_context(bool t_is_verbose = false) -> UniqueContext;
}

#endif
