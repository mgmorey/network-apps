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
#include "network/endpoint.hpp"                 // Endpoint
#include "network/endpointresult.hpp"           // EndpointResult
#include "network/format-ai-error.hpp"          // format_ai_error()
#include "network/get-sa-span.hpp"              // get_sa_span()
#include "network/hostname-length-limits.hpp"   // hostname_length_max
#include "network/oserrorresult.hpp"            // OsErrorResult
#include "network/quote.hpp"                    // quote()
#include "network/service-length-limits.hpp"    // service_length_max
#include "network/textbuffer.hpp"               // TextBuffer
#include "network/to-os-error.hpp"              // to_os_error()
#include "network/to-string-span-byte.hpp"      // to_string()

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
    const auto [sa, sa_length] {get_sa_span(bs)};
    const std::string_view hostname_sv {hostname.data(), hostname.size()};
    const std::string_view service_sv {service.data(), service.size()};

    if (is_verbose) {
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
        return OsErrorResult {os_error, oss.str()};
    }

    if (is_verbose) {
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

auto Network::get_endpointresult(std::span<const std::byte> bs, int flags,
                                 bool is_verbose) -> EndpointResult
{
    TextBuffer hostname {hostname_length_max};
    TextBuffer service {service_length_max};

    if (auto result {get_endpointresult(hostname,
                                        service,
                                        bs,
                                        flags,
                                        is_verbose)}) {
        return result;
    }

    return Endpoint {hostname, service};
}
