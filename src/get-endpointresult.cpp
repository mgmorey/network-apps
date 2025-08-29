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

#include "network/get-endpointresult.hpp"       // get_endpoint()
#include "network/bytespan.hpp"                 // ByteSpan
#include "network/charspan.hpp"                 // CharSpan
#include "network/endpoint.hpp"                 // Endpoint
#include "network/endpointresult.hpp"           // EndpointResult
#include "network/format-ai-error.hpp"          // format_ai_error()
#include "network/get-sa-span.hpp"              // get_sa_span()
#include "network/hostname-length-limits.hpp"   // hostname_length_max
#include "network/oserror.hpp"                  // OsError
#include "network/quote.hpp"                    // quote()
#include "network/service-length-limits.hpp"    // service_length_max
#include "network/sharedruntime.hpp"            // SharedRuntime
#include "network/textbuffer.hpp"               // TextBuffer
#include "network/to-os-error.hpp"              // to_os_error()
#include "network/to-string-bytespan.hpp"       // to_string()

#ifdef _WIN32
#include <ws2tcpip.h>   // ::getnameinfo()
#else
#include <netdb.h>      // ::getnameinfo()
#endif

#include <expected>     // std::unexpected
#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream
#include <string_view>  // std::string_view

auto Network::get_endpointresult(CharSpan hostname,
                                 CharSpan service,
                                 ByteSpan bs, int flags,
                                 const SharedRuntime& sr) -> OsError
{
    const auto [sa, sa_length] {get_sa_span(bs)};
    const std::string_view hostname_sv {hostname.data(), hostname.size()};
    const std::string_view service_sv {service.data(), service.size()};

    if (sr->is_verbose()) {
        // clang-format off
        std::cout << "Calling ::getnameinfo("
                  << to_string(bs)
                  << ", "
                  << sa_length
                  << ", "
                  << quote(hostname_sv)
                  << ", "
                  << hostname_sv.size()
                  << ", "
                  << quote(service_sv)
                  << ", "
                  << service_sv.size()
                  << ", "
                  << flags
                  << ')'
                  << std::endl;
        // clang-format on
    }

    if (const auto api_error {::getnameinfo(sa,
                                            sa_length,
                                            hostname.data(),
                                            hostname.size(),
                                            service.data(),
                                            service.size(),
                                            flags)}) {
        const auto os_error {to_os_error(api_error)};
        std::ostringstream oss;
        // clang-format off
        oss << "Call to ::getnameinfo("
            << to_string(bs)
            << ", "
            << sa_length
            << ", "
            << quote(hostname_sv)
            << ", "
            << hostname_sv.size()
            << ", "
            << quote(service_sv)
            << ", "
            << service_sv.size()
            << ", "
            << flags
            << ") returned "
            << api_error
            << " ("
            << format_ai_error(api_error)
            << ')';
        // clang-format on
        return {os_error, oss.str()};
    }

    if (sr->is_verbose()) {
        // clang-format off
        std::cout << "Call to ::getnameinfo("
                  << to_string(bs)
                  << ", "
                  << sa_length
                  << ", "
                  << quote(hostname_sv)
                  << ", "
                  << hostname_sv.size()
                  << ", "
                  << quote(service_sv)
                  << ", "
                  << service_sv.size()
                  << ", "
                  << flags
                  << ") returned data {"
                  << quote(hostname_sv)
                  << ", "
                  << quote(service_sv)
                  << '}'
                  << std::endl;
        // clang-format on
    }

    return {};
}

auto Network::get_endpointresult(ByteSpan bs, int flags,
                                 const SharedRuntime& sr) -> EndpointResult
{
    TextBuffer hostname {hostname_length_max};
    TextBuffer service {service_length_max};

    if (auto error {get_endpointresult(hostname,
                                       service,
                                       bs,
                                       flags,
                                       sr)}) {
        return std::unexpected {error};
    }

    return Endpoint {hostname, service};
}
