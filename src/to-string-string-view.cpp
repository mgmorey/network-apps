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

#include "network/to-string-string-view.h"      // to_string()
#include "network/string-null.h"                // string_null

#include <cctype>       // isprint()
#include <sstream>      // std::ostringstream
#include <string>       // std::string
#include <string_view>  // std::string_view

auto Network::to_string(const std::string_view& sv) -> std::string
{
    std::ostringstream oss;
    oss << std::oct;

    if (sv.data() == nullptr) {
        oss << string_null;
    }
    else {
        oss << "\"";

        for (const auto ch : sv) {
            if (isprint(ch) != 0) {
                oss << ch;
            }
            else if (ch != '\0') {
                oss << '\\'
                    << static_cast<unsigned>(ch);
            }
            else {
                break;
            }
        }

        oss << "\"";
    }

    return oss.str();
}
