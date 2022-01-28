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

#ifndef NETWORK_ADDRESSERROR_H
#define NETWORK_ADDRESSERROR_H

#include "network/bytestring.h"         // ByteString
#include "network/logicerror.h"         // LogicError

namespace Network
{
    class AddressError :
        public LogicError
    {
    public:
        explicit AddressError(const ByteString& t_str) noexcept;
        AddressError(const AddressError&) noexcept = default;
        AddressError(AddressError&&) noexcept = default;
        ~AddressError() noexcept override = default;
        auto operator=(const AddressError&) noexcept -> AddressError& = default;
        auto operator=(AddressError&&) noexcept -> AddressError& = default;
    };
}

#endif
