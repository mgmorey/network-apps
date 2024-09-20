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

#include "network/to-string-void.h"             // to_string()
#include "network/byte.h"                       // Byte
#include "network/to-string-span-byte.h"        // to_string()

#include <span>         // std::span
#include <string>       // std::string

auto Network::to_string(const void* data, length_type size) -> std::string
{
    return to_string(std::span(static_cast<const Byte*>(data), size));
}
