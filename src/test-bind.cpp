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
                                        // OsErrorResult, Socket,
                                        // SocketHints, SocketResult,
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
#include <optional>     // std:nullopt
#include <set>          // std::set
#include <type_traits>  // std::decay_t, std::is_same_v
#include <utility>      // std::move()
#include <variant>      // std::visit()

namespace Test
{
    using Network::Address;
    using Network::ByteString;
    using Network::Context;
    using Network::Endpoint;
    using Network::IpSocketHints;
    using Network::OsErrorResult;
    using Network::Socket;
    using Network::SocketHints;
    using Network::SocketResult;
    using Network::SocketResultVector;
    using Network::always_false_v;
    using Network::bind;
    using Network::os_error_type;
    using Network::parse;
    using Network::string_null;

    using ErrorCodeSet = std::set<os_error_type>;

    static constexpr auto handle_width {6};
    static constexpr auto localhost {"localhost"};

    static auto is_verbose {false};  // NOLINT

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
            std::visit([&](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;

                if constexpr (std::is_same_v<T, Socket>) {
                    test_socket(arg);
                }
                else if constexpr (std::is_same_v<T, OsErrorResult>) {
                    std::cerr << arg.string()
                              << std::endl;
                }
                else {
                    static_assert(always_false_v<T>, VISITOR_ERROR);
                }
            }, t_socket_result);
        }

        auto test_socket(const Socket& t_sock) -> void
        {
            const auto self {t_sock.sockname()};
            m_os << "Socket "
                 << std::right << std::setw(handle_width) << t_sock
                 << " bound on "
                 << m_endpoint.first.value_or(string_null)
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

    auto get_hints() -> SocketHints
    {
        return IpSocketHints {SOCK_STREAM, AI_PASSIVE | AI_CANONNAME};
    }

    auto parse_arguments(int argc, char** argv) -> Endpoint
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
            !operands.empty() ? operands[0] : localhost,
            {}
        };
    }

    auto print(const OsErrorResult& result) -> void
    {
        if (is_verbose) {
            std::cout << "Number: "
                      << result.number()
                      << std::endl
                      << "String: "
                      << result.string()
                      << std::endl;
        }
    }

    auto test_bind(const Endpoint& endpoint,
                   const SocketHints& hints,
                   const ErrorCodeSet& expected_codes) -> void
    {
        os_error_type actual_code {0};
        const auto bind_result {bind(endpoint, hints, is_verbose)};
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, SocketResultVector>) {
                std::for_each(arg.begin(), arg.end(),
                              Test(endpoint, std::cout));
            }
            else if constexpr (std::is_same_v<T, OsErrorResult>) {
                actual_code = arg.number();
                print(arg);
            }
            else {
                static_assert(always_false_v<T>, VISITOR_ERROR);
            }
        }, bind_result);
        assert(expected_codes.contains(actual_code));
    }

    auto test_bind_invalid_host(const Endpoint& endpoint,
                                const SocketHints& hints) -> void
    {
        test_bind(endpoint, hints, get_codes_invalid_host());
    }

    auto test_bind_valid(const Endpoint& endpoint,
                         const SocketHints& hints) -> void
    {
        test_bind(endpoint, hints, {0});
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace Test;

    const SocketHints hints {get_hints()};

    try {
        const auto context {Context::instance()};
        const auto endpoint {parse_arguments(argc, argv)};

        if (is_verbose) {
            std::cout << *context << std::endl;
        }

        if (getenv("http_proxy") == nullptr) {
            test_bind_valid(endpoint, hints);
        }

        const Endpoint invalid_host {".", {}};
        test_bind_invalid_host(invalid_host, hints);
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
