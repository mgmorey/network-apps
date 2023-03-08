// Copyright (C) 2023  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/context.h"            // Context, operator<<(),
                                        // std::endl, std::ostream

auto Network::operator<<(std::ostream& os,
                         const Context& context) -> std::ostream&
{
    const auto& description {context.m_description};
    const auto& status {context.m_system_status};
    const auto version {context.m_version};
    os << description;

    if (version) {
        os << " Version "
           << *version;
    }

    if (!status.empty()) {
        os << ' '
           << status;
    }

    return os;
}
