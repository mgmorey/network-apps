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
#include "network/buffer.h"                     // Buffer
#include "network/bytestring.h"                 // ByteString
#include "network/endpoint.h"                   // Endpoint
#include "network/endpointresult.h"             // EndpointResult
#include "network/format-ai-error.h"            // format_ai_error()
#include "network/get-sa-span.h"                // get_sa_span()
#include "network/hostname-length-limits.h"     // hostname_length_max
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/service-length-limits.h"      // service_length_max
#include "network/to-os-error.h"                // to_os_error()
#include "network/to-string-string-view.h"      // to_string()

#ifdef WIN32
#include <ws2tcpip.h>   // ::getnameinfo()
#else
#include <netdb.h>      // ::getnameinfo()
#endif

#include <iostream>     // std::cout, std::endl
#include <span>         // std::span
#include <sstream>      // std::ostringstream
#include <string_view>  // std::string_view

auto Network::get_endpointresult(const std::span<char>& hostname,
                                 const std::span<char>& service,
                                 const ByteString& addr, int flags,
                                 bool is_verbose) -> OsErrorResult
{
    const AddressString addr_str {addr};
    const auto [addr_ptr, addr_len] {get_sa_span(addr)};
    const std::string_view hostname_sv {hostname.data(), hostname.size()};
    const std::string_view service_sv {service.data(), service.size()};

    if (is_verbose) {
        // clang-format off
        std::cout << "Calling ::getnameinfo("
                  << addr_str
                  << ", "
                  << addr_len
                  << ", "
                  << to_string(hostname_sv)
                  << ", "
                  << hostname_sv.size()
                  << ", "
                  << to_string(service_sv)
                  << ", "
                  << service_sv.size()
                  << ", "
                  << flags
                  << ')'
                  << std::endl;
        // clang-format on
    }

    if (const auto api_error {::getnameinfo(addr_ptr,
                                            addr_len,
                                            hostname.data(),
                                            hostname.size(),
                                            service.data(),
                                            service.size(),
                                            flags)}) {
        const auto os_error {to_os_error(api_error)};
        std::ostringstream oss;
        // clang-format off
        oss << "Call to ::getnameinfo("
            << addr_str
            << ", "
            << addr_len
            << ", "
            << to_string(hostname_sv)
            << ", "
            << hostname_sv.size()
            << ", "
            << to_string(service_sv)
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
                  << addr_str
                  << ", "
                  << addr_len
                  << ", "
                  << to_string(hostname_sv)
                  << ", "
                  << hostname_sv.size()
                  << ", "
                  << to_string(service_sv)
                  << ", "
                  << service_sv.size()
                  << ", "
                  << flags
                  << ") returned data {"
                  << to_string(hostname_sv)
                  << ", "
                  << to_string(service_sv)
                  << '}'
                  << std::endl;
        // clang-format on
    }

    return {};
}

auto Network::get_endpointresult(const ByteString& addr, int flags,
                                 bool is_verbose) -> EndpointResult
{
    Buffer<char> hostname_buffer {hostname_length_max};
    Buffer<char> service_buffer {service_length_max};
    std::span<char> hostname_span {hostname_buffer};
    std::span<char> service_span {service_buffer};

    if (auto result {get_endpointresult(hostname_span,
                                        service_span,
                                        addr,
                                        flags,
                                        is_verbose)}) {
        return result;
    }

    return Endpoint {
        std::string {hostname_buffer},
        std::string {service_buffer}
    };
}
