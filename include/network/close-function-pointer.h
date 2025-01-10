// Copyright (C) 2025  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_CLOSE_FUNCTION_POINTER_H
#define NETWORK_CLOSE_FUNCTION_POINTER_H

#include "network/close-function-type.h"        // close_function_type

#ifdef WIN32
#include <winsock2.h>       // ::closesocket()
#else
#include <unistd.h>         // ::close()
#endif

namespace Network
{
#ifdef WIN32
    const close_function_type close_function_pointer {::closesocket};
#else
    const close_function_type close_function_pointer {::close};
#endif
}

#endif
