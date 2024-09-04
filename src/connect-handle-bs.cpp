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

#include "network/connect-handle-bs.h"          // connect()
#include "network/bytestring.h"                 // ByteString
#include "network/handle-type.h"                // handle_type
#include "network/open-handle.h"                // open()
#include "network/openhandleparams.h"           // OpenHandleParams
#include "network/openhandler.h"                // OpenHandler
#include "network/oserrorresult.h"              // OsErrorResult

#ifdef WIN32
#include <winsock2.h>       // ::connect()
#else
#include <sys/socket.h>     // ::connect()
#endif

auto Network::connect(handle_type handle,
                      const ByteString& addr,
                      bool is_verbose) -> OsErrorResult
{
    const OpenHandler handler {::connect, "::connect"};
    const OpenHandleParams args {handle, addr, is_verbose};
    return open(handler, args);
}
