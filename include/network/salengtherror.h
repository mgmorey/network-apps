// Copyright (C) 2023  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_SALENGTHERROR_H
#define NETWORK_SALENGTHERROR_H

#include "network/lengtherror.h"        // LengthError
#include "network/sa-len-type.h"        // sa_len_type

#include <string>       // std::string

namespace Network
{
    class SaLengthError :
        public LengthError
    {
    public:
        explicit SaLengthError(const std::string& t_value,
                               sa_len_type t_minimum,
                               sa_len_type t_maximum) noexcept;
        SaLengthError(const SaLengthError&) noexcept = default;
        SaLengthError(SaLengthError&&) noexcept = default;
        ~SaLengthError() noexcept override = default;
        auto operator=(const SaLengthError&) noexcept ->
            SaLengthError& = default;
        auto operator=(SaLengthError&&) noexcept ->
            SaLengthError& = default;
    };
}

#endif
