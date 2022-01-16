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

#ifndef NETWORK_LOGICERROR_H
#define NETWORK_LOGICERROR_H

#include "network/error.h"              // Error, std::move,
                                        // std::string

namespace Network
{
    class LogicError : public Error
    {
    public:
        explicit LogicError(std::string t_what) :
            Error(std::move(t_what))
        {
        }

        LogicError(const LogicError&) = default;
        LogicError(LogicError&&) noexcept = default;
        ~LogicError() override = default;
        auto operator=(const LogicError&) -> LogicError& = default;
        auto operator=(LogicError&&) noexcept -> LogicError& = default;
    };
}

#endif
