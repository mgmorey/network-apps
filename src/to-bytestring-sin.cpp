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

#include "network/to-bytestring-sin.h"  // to_bytestring()
#include "network/bytestring.h"         // ByteString
#include "network/to-bytespan-void.h"   // to_bytespan()
#include "network/to-bytestring-bs.h"   // to_bytestring()
#include "network/validate-sin.h"       // validate()

#ifdef WIN32
#include <winsock2.h>       // sockaddr_in
#else
#include <netinet/in.h>     // sockaddr_in
#endif

auto Network::to_bytestring(const sockaddr_in* sin) -> Network::ByteString
{
    validate(sin);
    return to_bytestring(to_bytespan(sin, sizeof *sin));
}
