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

#include "network/get-nameresult.h"     // Fd, GetNameHandler,
                                        // OsErrorResult, get_nameresult()
#include "network/bytestring.h"         // ByteString
#include "network/get-length.h"         // get_length()
#include "network/get-sa-pointer.h"     // get_sa_pointer()
#include "network/os-error.h"           // format_os_error(),
                                        // get_os_last_error(),
                                        // reset_os_last_error()
#include "network/socket-error.h"       // socket_error
#include "network/ss-sizes.h"           // ss_size

#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

auto Network::get_nameresult(const GetNameHandler& handler,
                             const GetNameParams& args) ->
    Network::ByteStringResult
{
    ByteString str {ss_size, Byte {}};
    auto length {get_length(str)};
    auto* pointer {get_sa_pointer(str)};
    reset_last_os_error();

    if (args.verbose) {
        std::cerr << "Calling "
                  << handler.second
                  << '('
                  << args.fd
                  << ", "
                  << str
                  << ", "
                  << static_cast<int>(length)
                  << ", ...)"
                  << std::endl;
    }

    const fd_type fd {static_cast<fd_type>(args.fd)};

    if (handler.first(fd, pointer, &length) == socket_error) {
        const auto error = get_last_os_error();
        std::ostringstream oss;
        oss << "Call to "
            << handler.second
            << '('
            << args.fd
            << ", "
            << str
            << ", "
            << static_cast<int>(length)
            << ", ...) failed with error "
            << error
            << ": "
            << format_os_error(error);
        return OsErrorResult {error, oss.str()};
    }

    str.resize(length);
    return str;
}