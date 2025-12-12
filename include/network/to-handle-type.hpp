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

#ifndef NETWORK_TO_HANDLE_TYPE_HPP
#define NETWORK_TO_HANDLE_TYPE_HPP

#include "network/handle-type-limits.hpp"       // handle_type_max,
                                                // handle_type_min
#include "network/handle-type.hpp"              // handle_type
#include "network/to-value.hpp"                 // to_value()

namespace Network
{
    auto to_handle_type(auto value) -> handle_type
    {
        return to_value<handle_type>("handle_type", value,
                                     handle_type_min,
                                     handle_type_max);
    }
}

#endif
