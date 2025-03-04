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

#include "network/get-endpointresult.h"         // get_endpoint()
#include "network/addressstring.h"              // AddressString
#include "network/endpoint.h"                   // Endpoint
#include "network/endpointresult.h"             // EndpointResult
#include "network/format-ai-error.h"            // format_ai_error()
#include "network/get-sa-span.h"                // get_sa_span()
#include "network/hostname-length-limits.h"     // hostname_length_max
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/quote.h"                      // quote()
#include "network/service-length-limits.h"      // service_length_max
#include "network/textbuffer.h"                 // TextBuffer
#include "network/to-os-error.h"                // to_os_error()

#ifdef WIN32
#include <ws2tcpip.h>   // ::getnameinfo()
#else
#include <netdb.h>      // ::getnameinfo()
#endif

#include <iostream>     // std::cout, std::endl
#include <span>         // std::span
#include <sstream>      // std::ostringstream
#include <string_view>  // std::string_view

auto Network::get_endpointresult(std::span<char> hostname,
                                 std::span<char> service,
                                 std::span<const std::byte> bs, int flags,
                                 bool is_verbose) -> OsErrorResult
{
    const AddressString sa_str {bs};
    const auto [sa, sa_length] {get_sa_span(bs)};
    const std::string_view hostname_sv {hostname.data(), hostname.size()};
    const std::string_view service_sv {service.data(), service.size()};

    if (is_verbose) {
        // clang-format off
        std::cout << "Calling ::getnameinfo("
                  << sa_str
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
            << sa_str
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
        return OsErrorResult {os_error, oss.str()};
    }

    if (is_verbose) {
        // clang-format off
        std::cout << "Call to ::getnameinfo("
                  << sa_str
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

auto Network::get_endpointresult(std::span<const std::byte> bs, int flags,
                                 bool is_verbose) -> EndpointResult
{
    TextBuffer hostname {hostname_length_max};
    TextBuffer service {service_length_max};

    if (auto result {get_endpointresult(std::span(hostname),
                                        std::span(service),
                                        bs,
                                        flags,
                                        is_verbose)}) {
        return result;
    }

    return Endpoint {hostname, service};
}
