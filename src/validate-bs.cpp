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

#include "network/validate-bs.h"        // validate
#include "network/bytestring.h"         // ByteString
#include "network/get-sa-length.h"      // get_sa_length()
#include "network/salengtherror.h"      // SaLengthError

#include <sstream>      // std::ostringstream

auto Network::validate(const ByteString& addr) -> void
{
    const auto length {addr.length()};
    const auto sa_len {get_sa_length(addr, length)};

    if (sa_len != length) {
        throw SaLengthError(std::to_string(sa_len), length, length);
    }
}
