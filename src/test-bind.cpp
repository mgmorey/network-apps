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
                                        // EndpointView,
                                        // IpSocketHints,
                                        // OsErrorResult, Socket,
                                        // SocketHints, SocketResult,
                                        // SocketResultVector,
                                        // UniqueSocket, bind(),
                                        // handle_null, handle_type,
                                        // os_error_type,
                                        // start_context(),
                                        // string_null
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
#include <type_traits>  // std::decay_t, std::is_same_v
#include <variant>      // std::visit()

namespace
{
    using Network::Address;
    using Network::ByteString;
    using Network::EndpointView;
    using Network::IpSocketHints;
    using Network::OsErrorResult;
    using Network::Socket;
    using Network::SocketHints;
    using Network::SocketResult;
    using Network::SocketResultVector;
    using Network::UniqueSocket;
    using Network::always_false_v;
    using Network::bind;
    using Network::handle_null;
    using Network::handle_type;
    using Network::os_error_type;
    using Network::parse;
    using Network::start_context;

    using ErrorCodeSet = std::set<os_error_type>;

    constexpr auto handle_width {6};
    constexpr auto localhost {"localhost"};
    constexpr IpSocketHints socket_hints
    {
        (SOCK_STREAM), (AI_PASSIVE | AI_CANONNAME)
    };

    auto is_verbose {false};  // NOLINT

    class Test
    {
    public:
        Test(const EndpointView& t_endpoint, std::ostream& t_os) :
            m_endpoint(t_endpoint),
            m_os(t_os)
        {
        }

        auto operator()(const SocketResult& t_socket_result) -> void
        {
            std::visit([&](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;

                if constexpr (std::is_same_v<T, UniqueSocket>) {
                    test_socket(*arg);
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
            assert(static_cast<bool>(t_sock));
            assert(static_cast<handle_type>(t_sock) != handle_null);
            const auto self {t_sock.sockname()};
            m_os << "Socket "
                 << std::right << std::setw(handle_width) << t_sock
                 << " bound to "
                 << Address(self)
                 << std::endl;
        }

    private:
        EndpointView m_endpoint;
        std::ostream& m_os;
    };

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
            !operands.empty() ? operands[0] : localhost,
            {}
        };
    }

    auto test_bind(const EndpointView& endpoint,
                   const SocketHints& hints) -> void
    {
        const auto bind_result {bind(endpoint, hints, is_verbose)};
        std::for_each(bind_result.begin(), bind_result.end(),
                      Test(endpoint, std::cout));
    }

    auto test_bind_valid(const EndpointView& endpoint,
                         const SocketHints& hints) -> void
    {
        test_bind(endpoint, hints);
    }

}


auto main(int argc, char* argv[]) -> int
{
    try {
        const auto endpoint {parse_arguments(argc, argv)};
        const auto context {start_context(is_verbose)};

        if (is_verbose) {
            std::cout << *context << std::endl;
        }

        if (getenv("http_proxy") == nullptr) {
            test_bind_valid(endpoint, socket_hints);
        }
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
