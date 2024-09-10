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

#include "network/assert.h"             // assert()
#include "network/network.h"            // Address, ByteString,
                                        // Context, Endpoint,
                                        // Hostname, OsErrorResult,
                                        // Socket, SocketHints,
                                        // SocketResult,
                                        // SocketResultVector,
                                        // connect(), get_hostname(),
                                        // os_error_type, string_null
#include "network/parse.h"              // parse()

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, PF_INET, PF_INET6,
                            // IPPROTO_IP, IPPROTO_TCP, IPPROTO_UDP,
                            // SOCK_DGRAM, SOCK_STREAM, WSAEFAULT,
                            // WSAENETUNREACH
#include <ws2tcpip.h>       // AI_ADDRCONFIG, AI_ALL, AI_CANONNAME
#else
#include <netdb.h>          // AI_ADDRCONFIG, AI_ALL, AI_CANONNAME
#include <sys/socket.h>     // AF_INET, AF_INET6, PF_INET, PF_INET6,
                            // SOCK_DGRAM, SOCK_STREAM
#endif

#include <algorithm>    // std::for_each()
#include <cerrno>       // EFAULT, EINVAL, EAFNOSUPPORT
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iomanip>      // std::right, std::setw()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <set>          // std::set
#include <type_traits>  // std::decay_t, std::is_same_v
#include <variant>      // std::visit()

namespace Test
{
    using Network::Address;
    using Network::ByteString;
    using Network::EndpointView;
    using Network::Hostname;
    using Network::HostnameView;
    using Network::IpSocketHints;
    using Network::OsErrorResult;
    using Network::PeerName;
    using Network::SharedSocket;
    using Network::Socket;
    using Network::SocketHints;
    using Network::SocketResult;
    using Network::SocketResultVector;
    using Network::always_false_v;
    using Network::connect;
    using Network::get_context;
    using Network::get_hostname;
    using Network::os_error_type;
    using Network::parse;
    using Network::string_null;

    using ErrorCodeSet = std::set<os_error_type>;

    static constexpr auto handle_width {6};
    static constexpr auto indent_width {handle_width + 18};
    static constexpr auto remotehost {"example.com"};
    static constexpr auto service {"http"};

    static auto is_verbose {false};  // NOLINT

    class Test
    {
    public:
        static auto get_codes_unreachable() -> const ErrorCodeSet&
        {
#if defined(WIN32)
            static const ErrorCodeSet codes {
                WSAENETUNREACH
            };
#else
            static const ErrorCodeSet codes {
                EACCES,
                EHOSTUNREACH,
                ENETDOWN,
                ENETUNREACH
            };
#endif
            return codes;
        }

        Test(const EndpointView& t_endpoint,
             const Hostname& t_hostname,
             std::ostream& t_os) :
            m_endpoint(t_endpoint),
            m_hostname(t_hostname),
            m_os(t_os)
        {
        }

        auto operator()(const SocketResult& t_socket_result) -> void
        {
            const auto& expected_codes {get_codes_unreachable()};
            std::visit([&](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;

                if constexpr (std::is_same_v<T, SharedSocket>) {
                    test_socket(*arg);
                }
                else if constexpr (std::is_same_v<T, OsErrorResult>) {
                    auto actual_code {arg.number()};

                    if (!expected_codes.contains(actual_code)) {
                        std::cerr << arg.string()
                                  << std::endl;
                    }
                }
                else {
                    static_assert(always_false_v<T>, VISITOR_ERROR);
                }
            }, t_socket_result);
        }

        auto test_socket(const Socket& t_sock) -> void
        {
            const auto peer {t_sock.peername()};
            const auto self {t_sock.sockname()};
            m_os << "Socket "
                 << std::right << std::setw(handle_width) << t_sock
                 << " connected "
                 << m_hostname
                 << " to "
                 << (m_endpoint.at(1).data() == nullptr ?
                     string_null : m_endpoint.at(1))
                 << " on "
                 << (m_endpoint.at(0).data() == nullptr ?
                     string_null : m_endpoint.at(0))
                 << std::endl
                 << "Socket "
                 << std::right << std::setw(handle_width) << t_sock
                 << " connected "
                 << Address(self)
                 << std::endl
                 << std::right << std::setw(indent_width) << "to "
                 << Address(peer)
                 << std::endl;
        }

    private:
        EndpointView m_endpoint;
        HostnameView m_hostname;
        std::ostream& m_os;
    };

    auto get_codes_invalid_host() -> const ErrorCodeSet&
    {
#if defined(WIN32)
        static const ErrorCodeSet codes {WSAHOST_NOT_FOUND};
#elif defined(OS_FREEBSD)
        static const ErrorCodeSet codes {EAI_AGAIN, EAI_NONAME};
#else
        static const ErrorCodeSet codes {EAI_AGAIN, EAI_NODATA, EAI_NONAME};
#endif
        return codes;
    }

    auto get_codes_invalid_service() -> const ErrorCodeSet&
    {
#if defined(WIN32)
        static const ErrorCodeSet codes {WSATYPE_NOT_FOUND};
#else
        static const ErrorCodeSet codes {EAI_NONAME, EAI_SERVICE};
#endif
        return codes;
    }

    auto get_hints() -> SocketHints
    {
        return IpSocketHints {SOCK_STREAM, AI_CANONNAME};
    }

    auto parse_arguments(int argc, char** argv) -> EndpointView
    {
        const auto [operands, options] {parse(argc, argv, "v")};

        if (options.contains('?')) {
            std::cerr << "Usage: "
                      << *argv
                      << " [-v]"
                      << std::endl;
            std::exit(EXIT_FAILURE);
        }

        if (options.contains('v')) {
            is_verbose = true;
        }

        return {
            !operands.empty() ? operands[0] : remotehost,
            operands.size() > 1 ? operands[1] : service
        };
    }

    auto print(const OsErrorResult& result) -> void
    {
        if (is_verbose) {
            std::cout << result.string()
                      << std::endl;
        }
    }

    auto test_connect(const EndpointView& endpoint,
                      const SocketHints& hints,
                      const Hostname& hostname,
                      const ErrorCodeSet& expected_codes) -> void
    {
        os_error_type actual_code {0};
        const auto connect_result {connect(endpoint, hints, is_verbose)};
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, SocketResultVector>) {
                std::for_each(arg.begin(), arg.end(),
                              Test(endpoint,
                                   hostname,
                                   std::cout));
            }
            else if constexpr (std::is_same_v<T, OsErrorResult>) {
                actual_code = arg.number();
                print(arg);
            }
            else {
                static_assert(always_false_v<T>, VISITOR_ERROR);
            }
        }, connect_result);
        assert(expected_codes.contains(actual_code));
    }

    auto test_connect_invalid_host(const EndpointView& endpoint,
                                   const SocketHints& hints,
                                   const Hostname& hostname) -> void
    {
        test_connect(endpoint, hints, hostname, get_codes_invalid_host());
    }

    auto test_connect_invalid_service(const EndpointView& endpoint,
                                      const SocketHints& hints,
                                      const Hostname& hostname) -> void
    {
        test_connect(endpoint, hints, hostname, get_codes_invalid_service());
    }

    auto test_connect_valid(const EndpointView& endpoint,
                            const SocketHints& hints,
                            const Hostname& hostname) -> void
    {
        test_connect(endpoint, hints, hostname, {0});
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace Test;

    const SocketHints hints {get_hints()};

    try {
        const auto endpoint {parse_arguments(argc, argv)};
        const auto context {get_context(true, is_verbose)};

        if (is_verbose) {
            std::cout << *context << std::endl;
        }

        const auto hostname {get_hostname()};
        const EndpointView invalid_host {".", service};
        test_connect_invalid_host(invalid_host, hints, hostname);
        const EndpointView invalid_service {remotehost, "."};
        test_connect_invalid_service(invalid_service, hints, hostname);

        if (getenv("http_proxy") == nullptr) {
            test_connect_valid(endpoint, hints, hostname);
        }
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
