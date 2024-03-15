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

#ifndef NETWORK_FAMILYERROR_H
#define NETWORK_FAMILYERROR_H

#include "network/logicerror.h"                 // LogicError
#include "network/socket-family-type.h"         // socket_family_type

namespace Network
{
    class FamilyError :
        public LogicError
    {
    public:
        explicit FamilyError(socket_family_type family) noexcept;
        FamilyError(const FamilyError&) noexcept = default;
        FamilyError(FamilyError&&) noexcept = default;
        ~FamilyError() noexcept override = default;
        auto operator=(const FamilyError&) noexcept -> FamilyError& = default;
        auto operator=(FamilyError&&) noexcept -> FamilyError& = default;
    };
}

#endif
