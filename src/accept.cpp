// Copyright (C) 2023  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/accept.hpp"                   // accept()
#include "network/acceptdata.hpp"               // AcceptData
#include "network/binarybuffer.hpp"             // BinaryBuffer
#include "network/error.hpp"                    // Error
#include "network/format-os-error.hpp"          // format_os_error()
#include "network/get-api-error.hpp"            // get_api_error()
#include "network/handle-null.hpp"              // handle_null
#include "network/reset-api-error.hpp"          // reset_api_error()
#include "network/socketcore.hpp"               // SocketCore
#include "network/to-os-error.hpp"              // to_os_error()
#include "network/to-string-bytespan.hpp"       // to_string()

#ifdef _WIN32
#include <winsock2.h>       // ::accept()
#else
#include <sys/socket.h>     // ::accept()
#endif

#include <iostream>     // std::cout, std::endl
#include <span>         // std::span
#include <sstream>      // std::ostringstream

auto Network::accept(const SocketCore& sc) -> AcceptData
{
    BinaryBuffer buffer;
    const std::span bs {buffer};
    auto [sa, sa_length] {buffer.span()};
    const auto handle_1 {sc.handle()};
    const auto is_verbose {sc.runtime()->is_verbose()};

    if (is_verbose) {
        // clang-format off
        std::cout << "Calling ::accept("
                  << handle_1
                  << ", "
                  << to_string(bs)
                  << ", "
                  << sa_length
                  << ')'
                  << std::endl;
        // clang-format on
    }

    reset_api_error();
    const auto handle_2 {::accept(handle_1, sa, &sa_length)};

    if (handle_2 == handle_null) {
        const auto api_error {get_api_error()};
        const auto os_error {to_os_error(api_error)};
        std::ostringstream oss;
        // clang-format off
        oss << "Call to ::accept("
            << handle_1
            << ", "
            << to_string(bs)
            << ", "
            << sa_length
            << ") failed with error "
            << api_error
            << ": "
            << format_os_error(os_error);
        // clang-format on
        throw Error(oss.str());
    }

    if (is_verbose) {
        const auto str {to_string(buffer)};
        // clang-format off
        std::cout << "Call to ::accept("
                  << handle_1
                  << ", "
                  << str
                  << ", "
                  << sa_length
                  << ") returned data "
                  << str
                  << std::endl;
        // clang-format on
    }

    return {*buffer, sc, handle_2};
}
