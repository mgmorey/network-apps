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

#ifndef NETWORK_ARGUMENTDATA_H
#define NETWORK_ARGUMENTDATA_H

#include "network/argument.h"           // Argument

#include <cstddef>      // std::size_t
#include <string>       // std::string
#include <vector>       // std::vector

namespace Network
{
    class ArgumentData
    {
    public:
        explicit ArgumentData(const std::vector<std::string>& data);
        ArgumentData(const ArgumentData&) = default;
        ArgumentData(ArgumentData&&) = default;
        ~ArgumentData();
        auto operator=(const ArgumentData&) -> ArgumentData& = default;
        auto operator=(ArgumentData&&) -> ArgumentData& = default;
        [[nodiscard]] auto data() -> Argument*;
        [[nodiscard]] auto size() const -> std::size_t;

    private:
        std::vector<Argument> m_args;
    };
}

#endif
