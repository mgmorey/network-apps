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

#include "network/get-hostnameresult.hpp"       // get_hostname()
#include "network/format-os-error.hpp"          // format_os_error()
#include "network/get-api-error.hpp"            // get_api_error()
#include "network/hostname-length-limits.hpp"   // hostname_length_max
#include "network/hostnameresult.hpp"           // HostnameResult
#include "network/oserror.hpp"                  // OsError
#include "network/quote.hpp"                    // quote()
#include "network/reset-api-error.hpp"          // reset_api_error()
#include "network/run.hpp"                      // run()
#include "network/sharedruntime.hpp"            // SharedRuntime
#include "network/textbuffer.hpp"               // TextBuffer
#include "network/to-name-length.hpp"           // to_name_length()
#include "network/to-os-error.hpp"              // to_os_error()

#ifdef WIN32
#include <winsock2.h>   // ::gethostname()
#else
#include <unistd.h>     // ::gethostname()
#endif

#include <expected>     // std::unexpected
#include <iostream>     // std::cout, std::endl
#include <span>         // std::span
#include <sstream>      // std::ostringstream
#include <string_view>  // std::string_view

auto Network::get_hostnameresult(std::span<char> hostname,
                                 const SharedRuntime& sr) -> OsError
{
    const std::string_view hostname_sv {hostname.data(), hostname.size()};

    if (sr->is_verbose()) {
        // clang-format off
        std::cout << "Calling ::gethostname("
                  << quote(hostname_sv)
                  << ", "
                  << hostname_sv.size()
                  << ')'
                  << std::endl;
        // clang-format on
    }

    reset_api_error();

    if (::gethostname(hostname.data(), to_name_length(hostname.size())) == -1) {
        const auto api_error {get_api_error()};
        const auto os_error {to_os_error(api_error)};
        std::ostringstream oss;
        // clang-format off
        oss << "Call to ::gethostname("
            << quote(hostname_sv)
            << ", "
            << hostname_sv.size()
            << ") failed with error "
            << api_error
            << ": "
            << format_os_error(os_error);
        // clang-format on
        return {os_error, oss.str()};
    }

    if (sr->is_verbose()) {
        // clang-format off
        std::cout << "Call to ::gethostname("
                  << quote(hostname_sv)
                  << ", "
                  << hostname_sv.size()
                  << ") returned data "
                  << quote(hostname_sv)
                  << std::endl;
        // clang-format on
    }

    return {};
}

auto Network::get_hostnameresult(const SharedRuntime& sr) -> HostnameResult
{
    TextBuffer hostname {hostname_length_max};

    if (auto error {get_hostnameresult(hostname, sr)}) {
        return std::unexpected {error};
    }

    return hostname;
}

auto Network::get_hostnameresult(bool is_verbose) -> HostnameResult
{
    const auto rt {run(is_verbose)};
    return get_hostnameresult(rt);
}
