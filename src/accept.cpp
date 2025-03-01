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

#include "network/accept.h"             // accept()
#include "network/acceptresult.h"       // AcceptResult
#include "network/addressstring.h"      // AddressString
#include "network/binarybuffer.h"       // BinaryBuffer
#include "network/error.h"              // Error
#include "network/format-os-error.h"    // format_os_error()
#include "network/get-api-error.h"      // get_api_error()
#include "network/get-sa-span.h"        // get_sa_span()
#include "network/handle-null.h"        // handle_null
#include "network/reset-api-error.h"    // reset_api_error()
#include "network/sa-length-limits.h"   // sa_length_max
#include "network/socketdata.h"         // SocketData
#include "network/to-os-error.h"        // to_os_error()
#include "network/to-size.h"            // to_size()

#ifdef WIN32
#include <winsock2.h>       // ::accept()
#else
#include <sys/socket.h>     // ::accept()
#endif

#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream

auto Network::accept(const SocketData& sd) -> AcceptResult
{
    BinaryBuffer buffer {sa_length_max};
    const auto handle_1 {sd.handle()};
    const auto is_verbose {sd.is_verbose()};
    const AddressString addr_str {buffer.get_view()};
    auto [addr_ptr, addr_len] {get_sa_span(buffer.span())};

    if (is_verbose) {
        std::cout << "Calling ::accept("
                  << handle_1
                  << ", "
                  << addr_str
                  << ", "
                  << addr_len
                  << ", ...)"
                  << std::endl;
    }

    reset_api_error();
    const auto handle_2 {::accept(handle_1, addr_ptr, &addr_len)};

    if (handle_2 == handle_null) {
        const auto api_error {get_api_error()};
        const auto os_error {to_os_error(api_error)};
        std::ostringstream oss;
        oss << "Call to ::accept("
            << handle_1
            << ", "
            << addr_str
            << ", "
            << addr_len
            << ", ...) failed with error "
            << api_error
            << ": "
            << format_os_error(os_error);
        throw Error(oss.str());
    }

    return {SocketData {sd, handle_2}, buffer.size(to_size(addr_len))};
}
