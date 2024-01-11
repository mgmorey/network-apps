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

#ifdef HAVE_SOCKADDR_SA_LEN
#include "network/get-sa-length.h"      // get_sa_length()
#include "network/logicerror.h"         // LogicError
#include "network/os-features.h"        // HAVE_SOCKADDR_SA_LEN

#include <sstream>      // std::ostringstream
#endif

auto Network::validate(const ByteString& addr) -> void
{
#ifdef HAVE_SOCKADDR_SA_LEN
    const auto length {addr.length()};
    const auto sa_len {get_sa_length(addr)};

    if (sa_len != length) {
        std::ostringstream oss;
        oss << "Stored socket length "
            << static_cast<size_t>(sa_len)
            << " differs from actual length "
            << static_cast<size_t>(length);
        throw LogicError(oss.str());
    }
#else
    static_cast<void>(addr);
#endif
}
