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

#ifndef NETWORK_RUNTIMEDATA_HPP
#define NETWORK_RUNTIMEDATA_HPP

#include "network/failmode.hpp"                 // FailMode
#ifdef WIN32
#include "network/optionalversion.hpp"          // OptionalVersion
#endif

namespace Network
{
    struct RuntimeData
    {
#ifdef WIN32
        RuntimeData(OptionalVersion t_version,
                    FailMode t_fail_mode,
                    bool t_is_verbose) :
            m_version(t_version),
            m_fail_mode(t_fail_mode),
            m_is_verbose(t_is_verbose)
        {
        }
#endif
        RuntimeData(FailMode t_fail_mode,
                    bool t_is_verbose) :
            m_fail_mode(t_fail_mode),
            m_is_verbose(t_is_verbose)
        {
        }
        explicit RuntimeData(bool t_is_verbose = false) :
            m_is_verbose(t_is_verbose)
        {
        }

#ifdef WIN32
        OptionalVersion m_version;  // NOLINT
#endif
        FailMode m_fail_mode {FailMode::throw_error};  // NOLINT
        bool m_is_verbose {false};  // NOLINT
    };
}

#endif
