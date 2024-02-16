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

#ifndef NETWORK_TO_BYTESTRING_AUTO_H
#define NETWORK_TO_BYTESTRING_AUTO_H

#include "network/bytestring.h"                 // ByteString
#include "network/sa-len-type.h"                // sa_len_type
#include "network/to-bytespan.h"                // to_bytespan()
#include "network/to-bytestring-ai.h"           // to_bytestring()
#ifndef WIN32
#include "network/to-bytestring-char.h"         // to_bytestring()
#include "network/to-bytestring-path.h"         // to_bytestring()
#endif
#include "network/to-bytestring-span.h"         // to_bytestring()

namespace Network
{
    auto to_bytestring(const auto* pointer,
                       sa_len_type length) -> Network::ByteString
    {
        return to_bytestring(to_bytespan(pointer, length));
    }

    auto to_bytestring(const auto* pointer) -> Network::ByteString
    {
        return to_bytestring(pointer, sizeof *pointer);
    }
}

#endif
