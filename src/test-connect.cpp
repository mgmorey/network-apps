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
                                        // Hostname, OptionalHostname,
                                        // OsErrorResult, Overloaded,
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
#include <netinet/in.h>     // IPPROTO_IP, IPPROTO_TCP, IPPROTO_UDP
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
#include <string>       // std::string
#include <utility>      // std::move()
#include <variant>      // std::visit()

namespace TestConnect
{
    using Network::Address;
    using Network::ByteString;
    using Network::Context;
    using Network::Endpoint;
    using Network::Hostname;
    using Network::OptionalHostname;
    using Network::OsErrorResult;
    using Network::Overloaded;
    using Network::PeerName;
    using Network::Socket;
    using Network::SocketHints;
    using Network::SocketResult;
    using Network::SocketResultVector;
    using Network::connect;
    using Network::get_hostname;
    using Network::os_error_type;
    using Network::parse;
    using Network::string_null;

    using ErrorCodeSet = std::set<os_error_type>;

    static constexpr auto handle_width {6};
    static constexpr auto indent_width {handle_width + 18};
    static constexpr auto localhost {"example.com"};
    static constexpr auto localservice {"http"};

    static bool verbose {false};  // NOLINT

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

        Test(Endpoint t_endpoint,
             OptionalHostname t_hostname,
             std::ostream& t_os) :
            m_endpoint(std::move(t_endpoint)),
            m_hostname(std::move(t_hostname)),
            m_os(t_os)
        {
        }

        auto operator()(const SocketResult& t_socket_result) -> void
        {
            const auto& expected_codes {get_codes_unreachable()};
            std::visit(Overloaded {
                    [&](const Socket& sock) {
                        test_socket(sock);
                    },
                    [&](const OsErrorResult& error) {
                        auto actual_code {error.number()};

                        if (!expected_codes.contains(actual_code)) {
                            std::cerr << error.string()
                                      << std::endl;
                        }
                    }
                }, t_socket_result);
        }

        auto test_socket(const Socket& t_sock) -> void
        {
            const auto hostname {m_endpoint.first};
            const auto service {m_endpoint.second};
            const auto peer {t_sock.peername()};
            const auto self {t_sock.sockname()};
            m_os << "Socket "
                 << std::right << std::setw(handle_width) << t_sock
                 << " connected "
                 << m_hostname.value_or(string_null)
                 << " to "
                 << service.value_or(string_null)
                 << " on "
                 << hostname.value_or(string_null)
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
        Endpoint m_endpoint;
        OptionalHostname m_hostname;
        std::ostream& m_os;
    };

    static auto get_codes_invalid_host() -> const ErrorCodeSet&
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

    static auto get_codes_invalid_service() -> const ErrorCodeSet&
    {
#if defined(WIN32)
        static const ErrorCodeSet codes {WSATYPE_NOT_FOUND};
#else
        static const ErrorCodeSet codes {EAI_NONAME, EAI_SERVICE};
#endif
        return codes;
    }

    static auto parse(int argc, char** argv) -> Network::Endpoint
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
            verbose = true;
        }

        return {
            !operands.empty() ? operands[0] : localhost,
            operands.size() > 1 ? operands[1] : localservice
        };
    }

    static auto print(const OsErrorResult& result,
                      const std::string& description = "") -> void
    {
        if (verbose) {
            std::cout << "Error result"
                      << (description.empty() ? "" : ": " + description)
                      << std::endl
                      << "    Number: "
                      << result.number()
                      << std::endl
                      << "    String: "
                      << result.string()
                      << std::endl;
        }
    }

    static auto test_connect_invalid_host(const Endpoint& endpoint,
                                          const SocketHints& hints) -> void
    {
        os_error_type actual_code {0};
        const auto& expected_codes {get_codes_invalid_host()};
        const auto connect_result {connect(endpoint, hints, verbose)};
        std::visit(Overloaded {
                [&](const SocketResultVector& socket_results) {
                    static_cast<void>(socket_results);
                },
                [&](const OsErrorResult& error) {
                    actual_code = error.number();
                    print(error, "connect() with invalid host");
                }
            }, connect_result);
        assert(expected_codes.contains(actual_code));
    }

    static auto test_connect_invalid_service(const Endpoint& endpoint,
                                             const SocketHints& hints) -> void
    {
        os_error_type actual_code {0};
        const auto& expected_codes {get_codes_invalid_service()};
        const auto connect_result {connect(endpoint, hints, verbose)};
        std::visit(Overloaded {
                [&](const SocketResultVector& socket_results) {
                    static_cast<void>(socket_results);
                },
                [&](const OsErrorResult& error) {
                    actual_code = error.number();
                    print(error, "connect() with invalid service");
                }
            }, connect_result);
        assert(expected_codes.contains(actual_code));
    }

    static auto test_connect_valid(const Endpoint& endpoint,
                                   const SocketHints& hints,
                                   const Hostname& hostname) -> void
    {
        const auto connect_result {connect(endpoint, hints, verbose)};
        std::visit(Overloaded {
                [&](const SocketResultVector& socket_results) {
                    std::for_each(socket_results.begin(), socket_results.end(),
                                  Test(endpoint,
                                       hostname,
                                       std::cout));
                },
                [&](const OsErrorResult& error) {
                    print(error, "connect() with valid endpoint");
                }
            }, connect_result);
    }

    static auto test_connect_valid(const Endpoint& endpoint,
                                   const SocketHints& hints) -> void
    {
        test_connect_valid(endpoint, hints, get_hostname());
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace TestConnect;

    static constexpr SocketHints hints
        {AI_CANONNAME, AF_UNSPEC, SOCK_STREAM, 0};

    try {
        const auto& context {Context::instance()};
        const auto endpoint {parse(argc, argv)};

        if (verbose) {
            std::cout << context << std::endl;
        }

        test_connect_valid(endpoint, hints);
        const Endpoint invalid_host {".", localservice};
        test_connect_invalid_host(invalid_host, hints);
        const Endpoint invalid_service {localhost, "."};
        test_connect_invalid_service(invalid_service, hints);
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
