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

#ifndef NETWORK_RESULT_H
#define NETWORK_RESULT_H

#include "network/result-type.h"        // result_type

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    struct Result
    {
        Result() = default;
        Result(result_type t_result, std::string t_string);
        operator bool() const;  // NOLINT
        [[nodiscard]] auto result() const -> result_type;
        [[nodiscard]] auto string() const -> std::string;

    private:
        result_type m_result {0};
        std::string m_string;
    };

    extern auto operator<<(std::ostream& os,
                           const Result& result) -> std::ostream&;
}

#endif
