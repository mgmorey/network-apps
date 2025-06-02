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

#include "network/runtimedata.hpp"              // RuntimeData
#include "network/failmode.hpp"                 // FailMode
#include "network/optionalversion.hpp"          // OptionalVersion

Network::RuntimeData::RuntimeData(OptionalVersion t_version,
                                  FailMode t_fail_mode,
                                  bool t_is_verbose) noexcept :
    m_fail_mode(t_fail_mode),
    m_is_verbose(t_is_verbose),
    m_version(t_version)
{
}

Network::RuntimeData::RuntimeData(FailMode t_fail_mode,
                                  bool t_is_verbose) noexcept :
    m_fail_mode(t_fail_mode),
    m_is_verbose(t_is_verbose)
{
}

Network::RuntimeData::RuntimeData(bool t_is_verbose) noexcept :
    m_is_verbose(t_is_verbose)
{
}

auto Network::RuntimeData::fail_mode() const noexcept -> FailMode
{
    return m_fail_mode;
}

auto Network::RuntimeData::is_verbose() const noexcept -> bool
{
    return m_is_verbose;
}

auto Network::RuntimeData::version() const noexcept -> OptionalVersion
{
    return m_version;
}
