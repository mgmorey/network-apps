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

#include "network/get-nameresult.h"             // get_nameresult()
#include "network/binarybuffer.h"               // BinaryBuffer
#include "network/bytestringresult.h"           // ByteStringResult
#include "network/format-os-error.h"            // format_os_error()
#include "network/get-api-error.h"              // get_api_error()
#include "network/get-sa-span.h"                // get_sa_span()
#include "network/getnamehandler.h"             // GetNameHandler
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/reset-api-error.h"            // reset_api_error()
#include "network/sa-length-limits.h"           // sa_length_max
#include "network/socket-error.h"               // socket_error
#include "network/socketdata.h"                 // SocketData
#include "network/to-os-error.h"                // to_os_error()
#include "network/to-size.h"                    // to_size()
#include "network/to-string-span-byte.h"        // to_string()

#ifdef WIN32
#include <winsock2.h>       // ::getpeername(), ::getsockname()
#else
#include <sys/socket.h>     // ::getpeername(), ::getsockname()
#endif

#include <array>        // std::arrray
#include <iostream>     // std::cout, std::endl
#include <span>         // std::span
#include <sstream>      // std::ostringstream
#include <utility>      // std::make_pair

namespace
{
    auto get_binding(bool is_bind) -> Network::GetNameHandler
    {
        static const std::array<Network::GetNameHandler, 2> bindings {
            std::make_pair(::getsockname, "::getsockname"),
            std::make_pair(::getpeername, "::getpeername"),
        };

        return bindings.at(static_cast<std::size_t>(is_bind));
    }
}

auto Network::get_nameresult(const SocketData& sd, bool is_peer) ->
    ByteStringResult
{
    const auto binding {get_binding(is_peer)};
    BinaryBuffer buffer {sa_length_max};
    const std::span bs {buffer};
    auto [sa, sa_length] {get_sa_span(bs)};
    const auto handle {sd.handle()};

    if (sd.is_verbose()) {
        // clang-format off
        std::cout << "Calling "
                  << binding.second
                  << '('
                  << handle
                  << ", "
                  << to_string(bs)
                  << ", "
                  << sa_length
                  << ')'
                  << std::endl;
        // clang-format on
    }

    reset_api_error();

    if (binding.first(handle, sa, &sa_length) == socket_error) {
        const auto api_error {get_api_error()};
        const auto os_error {to_os_error(api_error)};
        std::ostringstream oss;
        // clang-format off
        oss << "Call to "
            << binding.second
            << '('
            << handle
            << ", "
            << to_string(bs)
            << ", "
            << sa_length
            << ") failed with error "
            << api_error
            << ": "
            << format_os_error(os_error);
        // clang-format on
        return OsErrorResult {os_error, oss.str()};
    }

    buffer.resize(to_size(sa_length));

    if (sd.is_verbose()) {
        const auto str {to_string(std::span<std::byte>(buffer))};
        // clang-format off
        std::cout << "Call to "
                  << binding.second
                  << '('
                  << handle
                  << ", "
                  << str
                  << ", "
                  << sa_length
                  << ") returned data "
                  << str
                  << std::endl;
        // clang-format on
    }

    return *buffer;
}
