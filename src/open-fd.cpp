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

#include "network/open-fd.h"            // ByteString, Fd,
                                        // OpenHandler, OsErrorResult,
                                        // fd_type, open(),
                                        // operator<<()
#include "network/get-length.h"         // get_length()
#include "network/get-sa-pointer.h"     // get_sa_pointer()
#include "network/optionalstring.h"     // OptionalString
#include "network/os-error.h"           // format_os_error(),
                                        // get_last_os_error(),
                                        // reset_last_os_error()
#include "network/socket-error.h"       // socket_error
#include "network/to-string.h"          // to_string()

#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream

static auto format(const Network::ByteString& addr,
                   Network::OptionalString& str) -> std::string
{
    if (!str) {
        str = Network::to_string(addr);
    }

    return *str;
}

auto Network::open(const OpenHandler& handler,
                   const OpenFdParams& args) -> Network::OsErrorResult
{
    const auto* const pointer {get_sa_pointer(args.str)};
    const auto length {get_length(args.str)};
    reset_last_os_error();
    OptionalString str;

    if (args.verbose) {
        std::cout << "Calling "
                  << handler.second
                  << '('
                  << args.fd
                  << ", "
                  << format(args.str, str)
                  << ", "
                  << static_cast<int>(length)
                  << ')'
                  << std::endl;
    }

    const fd_type handle {static_cast<fd_type>(args.fd)};

    if (handler.first(handle, pointer, length) == socket_error) {
        const auto error = get_last_os_error();
        std::ostringstream oss;
        oss << "Call to "
            << handler.second
            << '('
            << args.fd
            << ", "
            << format(args.str, str)
            << ", "
            << static_cast<int>(length)
            << ") failed with error "
            << error
            << ": "
            << format_os_error(error);
        return OsErrorResult {error, oss.str()};
    }

    return OsErrorResult {};
}
