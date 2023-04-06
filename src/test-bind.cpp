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
                                        // OsErrorResult, Overloaded,
                                        // Socket, SocketHints,
                                        // SocketResult,
                                        // SocketResultVector, bind(),
                                        // os_error_type, string_null
#include "network/parse.h"              // parse()

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, PF_INET, PF_INET6,
                            // IPPROTO_IP, IPPROTO_TCP, IPPROTO_UDP,
                            // SOCK_DGRAM, SOCK_STREAM
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

namespace TestBind
{
    using Network::Address;
    using Network::ByteString;
    using Network::Context;
    using Network::Endpoint;
    using Network::OsErrorResult;
    using Network::Overloaded;
    using Network::Socket;
    using Network::SocketHints;
    using Network::SocketResult;
    using Network::SocketResultVector;
    using Network::bind;
    using Network::os_error_type;
    using Network::parse;
    using Network::string_null;

    using ErrorCodeSet = std::set<os_error_type>;

    static constexpr auto handle_width {6};
    static constexpr auto localhost {"localhost"};
    static constexpr auto localservice {"8085"};

    static bool verbose {false};  // NOLINT

    class Test
    {
    public:
        Test(Endpoint t_endpoint, std::ostream& t_os) :
            m_endpoint(std::move(t_endpoint)),
            m_os(t_os)
        {
        }

        auto operator()(const SocketResult& t_socket_result) -> void
        {
            std::visit(Overloaded {
                    [&](const Socket& sock) {
                        test_socket(sock);
                    },
                        [&](const OsErrorResult& error) {
                            std::cerr << error.string()
                                      << std::endl;
                        }
                        }, t_socket_result);
        }

        auto test_socket(const Socket& t_sock) -> void
        {
            const auto hostname {m_endpoint.first};
            const auto service {m_endpoint.second};
            const auto self {t_sock.sockname()};
            m_os << "Socket "
                 << std::right << std::setw(handle_width) << t_sock
                 << " bound to "
                 << service.value_or(string_null)
                 << " on "
                 << hostname.value_or(string_null)
                 << std::endl
                 << "Socket "
                 << std::right << std::setw(handle_width) << t_sock
                 << " bound to "
                 << Address(self)
                 << std::endl;
        }

    private:
        Endpoint m_endpoint;
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

    auto parse(int argc, char** argv) -> Network::Endpoint
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

    auto print(const OsErrorResult& result,
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

    auto test_bind_invalid_host(const Endpoint& endpoint,
                                const SocketHints& hints) -> void
    {
        os_error_type actual_code {0};
        const auto& expected_codes {get_codes_invalid_host()};
        const auto bind_result {bind(endpoint, hints, verbose)};
        std::visit(Overloaded {
                [&](const SocketResultVector& socket_results) {
                    static_cast<void>(socket_results);
                },
                    [&](const OsErrorResult& error) {
                        actual_code = error.number();
                        print(error, "bind() with invalid host");
                    }
                    }, bind_result);
        assert(expected_codes.contains(actual_code));
    }

    auto test_bind_invalid_service(const Endpoint& endpoint,
                                   const SocketHints& hints) -> void
    {
        os_error_type actual_code {0};
        const auto& expected_codes {get_codes_invalid_service()};
        const auto bind_result {bind(endpoint, hints, verbose)};
        std::visit(Overloaded {
                [&](const SocketResultVector& socket_results) {
                    static_cast<void>(socket_results);
                },
                    [&](const OsErrorResult& error) {
                        actual_code = error.number();
                        print(error, "bind() with invalid service");
                    }
                    }, bind_result);
        assert(expected_codes.contains(actual_code));
    }

    auto test_bind_valid(const Endpoint& endpoint,
                         const SocketHints& hints) -> void
    {
        const auto bind_result {bind(endpoint, hints, verbose)};
        std::visit(Overloaded {
                [&](const SocketResultVector& socket_results) {
                    std::for_each(socket_results.begin(), socket_results.end(),
                                  Test(endpoint, std::cout));
                },
                    [&](const OsErrorResult& error) {
                        print(error, "bind() with valid endpoint");
                    }
                    }, bind_result);
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace TestBind;

    static constexpr SocketHints hints
        {AI_CANONNAME, AF_UNSPEC, SOCK_STREAM, 0};

    try {
        const auto& context {Context::instance()};
        const auto endpoint {parse(argc, argv)};

        if (verbose) {
            std::cout << context << std::endl;
        }

        test_bind_valid(endpoint, hints);
        const Endpoint invalid_host {".", localservice};
        test_bind_invalid_host(invalid_host, hints);
        const Endpoint invalid_service {localhost, "."};
        test_bind_invalid_service(invalid_service, hints);
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
