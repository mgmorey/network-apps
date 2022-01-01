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

#include "network/get-name.h"           // Bytes, Fd, GetNameHandler,
                                        // Result, get_name()
#include "network/error.h"              // format_error(),
                                        // get_last_error(),
                                        // reset_last_error()
#include "network/get-length.h"         // Bytes, get_length()
#include "network/get-sa-pointer.h"     // Bytes, get_sa_pointer()
#include "network/get-sockaddr.h"       // Bytes, get_sockaddr()
#include "network/is-valid.h"           // Bytes, is_valid()

#include <algorithm>    // std::max()
#include <cassert>      // assert()
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

auto Network::get_name(const GetNameHandler& handler, Fd fd,
                       bool verbose) -> Network::BytesResult
{
    Result result;
    auto addr {get_sockaddr()};
    auto addr_len {get_length(addr)};
    auto *addr_ptr {get_sa_pointer(addr)};

    if (verbose) {
        std::cerr << "Calling "
                  << handler.second
                  << '('
                  << fd
                  << ", "
                  << addr
                  << ", "
                  << static_cast<int>(addr_len)
                  << ", ...)"
                  << std::endl;
    }

    reset_last_error();

    if (handler.first(fd, addr_ptr, &addr_len) != 0) {
        auto error = get_last_error();
        std::ostringstream oss;
        oss << "Call to "
            << handler.second
            << '('
            << fd
            << ", "
            << addr
            << ", "
            << static_cast<int>(addr_len)
            << ", ...) failed with error "
            << error
            << ": "
            << format_error(error);
        return Result(error, oss.str());
    }

    addr.resize(addr_len);
    assert(is_valid(addr, verbose));  // NOLINT
    return addr;
}
