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

#ifndef NETWORK_ERROR_H
#define NETWORK_ERROR_H

#include "network/error-type.h"         // error_type

#include <string>       // std::string

namespace Network
{
    extern auto format_error(error_type error) -> std::string;
    extern auto get_last_error() -> error_type;
    extern auto reset_last_error() -> error_type;
}

#endif
