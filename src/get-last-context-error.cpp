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

#include "network/get-last-context-error.h" // context_error_type,
                                            // get_last_context_error()

#ifdef WIN32
#include <winsock2.h>       // WSAGetLastError()
#else
#include <cerrno>           // errno
#endif

auto Network::get_last_context_error() -> Network::context_error_type
{
    context_error_type context_error {0};
#ifdef WIN32
    context_error = ::WSAGetLastError();
#else
    context_error = errno;
#endif
    return context_error;
}
