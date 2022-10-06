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

#include "network/get-last-os-error.h"      // get_last_os_error(),
                                            // os_error_type
#include "network/rangeerror.h"             // RangeError

#ifdef WIN32
#include <winsock2.h>       // WSAGetLastError()
#else
#include <cerrno>           // errno
#endif

#include <climits>      // INT_MAX, INT_MIN
#include <sstream>      // std::ostringstream

auto Network::get_last_os_error() -> Network::os_error_type
{
    os_error_type os_error {0};
#ifdef WIN32
    const auto error {::WSAGetLastError()};

    if (error < 0 || error > INT_MAX) {
        std::ostringstream oss;
        oss << "Value "
            << error
            << " is out of range ["
            << 0
            << ", "
            << INT_MAX
            << "] of os_error_type";
        throw RangeError(oss.str());
    }

    os_error = static_cast<os_error_type>(error);
#else
    os_error = errno;
#endif
    return os_error;
}
