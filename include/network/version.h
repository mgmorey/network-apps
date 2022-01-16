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

#ifndef NETWORK_VERSION_H
#define NETWORK_VERSION_H

#include "network/version-type.h"       // version_type

#include <ostream>     // std::ostream

namespace Network
{
    class Version
    {
        friend auto operator<<(std::ostream& os, Version version) -> std::ostream&;

    public:
        // cppcheck-suppress noExplicitConstructor
        Version(version_type t_version);  // NOLINT
        Version(version_type t_major, version_type t_minor);
        Version(const Version&) = default;
        Version(Version&&) noexcept = default;
        ~Version() = default;
        auto operator=(const Version&) -> Version& = default;
        auto operator=(Version&&) noexcept -> Version& = default;
        operator version_type() const;  // NOLINT

    private:
        static constexpr auto m_radix {0x100U};
        version_type m_value {0x0U};
    };

    extern auto operator<<(std::ostream& os, Version version) -> std::ostream&;
}

#endif
