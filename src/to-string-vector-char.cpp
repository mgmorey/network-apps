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

#include "network/to-string-vector-char.h"      // to_string()

#include <algorithm>    // std::find()
#include <string>       // std::string
#include <vector>       // std::vector

auto Network::to_string(std::vector<char>::const_iterator cbegin,
                        std::vector<char>::const_iterator cend,
                        bool is_text) -> std::string
{
    if (is_text) {
        cend = std::find(cbegin, cend, '\0');
    }

    return {cbegin, cend};
}

auto Network::to_string(const std::vector<char>& v,
                        bool is_text) -> std::string
{
    return to_string(v.cbegin(), v.cend(), is_text);
}
