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

#include "network/get-nameresult.hpp"           // get_nameresult()
#include "network/binarybuffer.hpp"             // BinaryBuffer
#include "network/bytestringresult.hpp"         // ByteStringResult
#include "network/format-os-error.hpp"          // format_os_error()
#include "network/get-api-error.hpp"            // get_api_error()
#include "network/namehandler.hpp"              // NameHandler
#include "network/oserrorresult.hpp"            // OsErrorResult
#include "network/reset-api-error.hpp"          // reset_api_error()
#include "network/socket-error.hpp"             // socket_error
#include "network/socketdata.hpp"               // SocketData
#include "network/to-os-error.hpp"              // to_os_error()
#include "network/to-string-span-byte.hpp"      // to_string()

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
    auto get_binding(bool is_sockname) -> Network::NameHandler
    {
        static const std::array<Network::NameHandler, 2> bindings {
            std::make_pair(::getpeername, "::getpeername"),
            std::make_pair(::getsockname, "::getsockname"),
        };

        return bindings.at(static_cast<std::size_t>(is_sockname));
    }
}

auto Network::get_nameresult(const SocketData& sd, bool is_sockname) ->
    ByteStringResult
{
    const auto binding {get_binding(is_sockname)};
    BinaryBuffer buffer;
    const std::span bs {buffer};
    auto [sa, sa_length] {buffer.span()};
    const auto handle {sd.handle()};
    const auto is_verbose {sd.runtime()->is_verbose()};

    if (is_verbose) {
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

    if (is_verbose) {
        const auto str {to_string(buffer)};
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
