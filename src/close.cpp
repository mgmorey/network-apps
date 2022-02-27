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

#include "network/close.h"              // Fd, fd_null, fd_type

#ifdef WIN32
#include <winsock2.h>   // closesocket()
#else
#include <unistd.h>     // close()
#endif

#include <iostream>     // std::cout, std::endl

auto Network::close(fd_type fd, bool verbose) noexcept -> Network::fd_type
{
    if (fd != fd_null) {
        if (verbose) {
            std::cout << "Calling "
#ifdef WIN32
                      << "::closesocket"
#else
                      << "::close"
#endif
                      << '('
                      << fd
                      << ')'
                      << std::endl;
        }

#ifdef WIN32
        ::closesocket(fd);
#else
        ::close(fd);
#endif
    }

    return fd_null;
}

auto Network::close(Fd fd, bool verbose) noexcept -> fd_type
{
    return Network::close(static_cast<fd_type>(fd), verbose);
}
