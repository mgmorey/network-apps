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

#include "network/assert.hpp"           // assert()
#include "network/network.hpp"          // Address, ByteString,
                                        // EndpointView, Error,
                                        // Hostname, HostnameView,
                                        // IpSocketHints, Socket,
                                        // SocketHints, SocketResult,
                                        // UniqueSocket, connect(),
                                        // get_hostname(),
                                        // os_error_type, run(),
                                        // string_null
#include "network/parse.hpp"            // parse()

#ifdef _WIN32
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

#include <algorithm>    // std::ranges
#include <cerrno>       // EFAULT, EINVAL, EAFNOSUPPORT
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <iomanip>      // std::right, std::setw()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <set>          // std::set

namespace
{
    using Network::Address;
    using Network::ByteString;
    using Network::EndpointView;
    using Network::Error;
    using Network::Hostname;
    using Network::HostnameView;
    using Network::IpSocketHints;
    using Network::Socket;
    using Network::SocketHints;
    using Network::SocketResult;
    using Network::Symbol;
    using Network::UniqueSocket;
    using Network::connect;
    using Network::get_hostname;
    using Network::os_error_type;
    using Network::parse;
    using Network::run;
    using Network::string_null;

    using ErrorCodeSet = std::set<os_error_type>;

    constexpr auto handle_width {6};
    constexpr auto indent_width {handle_width + 18};
    constexpr auto remotehost {"google.com"};
    constexpr auto service {"http"};
    constexpr IpSocketHints socket_hints
    {
        (SOCK_STREAM), (AI_CANONNAME)
    };

    auto is_verbose {false};  // NOLINT

    class Test
    {
    public:
        static auto get_codes_unreachable() -> const ErrorCodeSet&
        {
#ifdef _WIN32
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

        auto operator()(const SocketResult& t_result) -> void
        {
            if (t_result) {
                test(**t_result);
            }
            else {
                const auto& expected_codes {get_codes_unreachable()};

                if (!expected_codes.contains(t_result.error().number())) {
                    std::cerr << t_result.error().string()
                              << std::endl;
                }
            }
        }

        auto test(const Socket& t_sock) -> void
        {
            const Address host {t_sock.get_name(Symbol::connect)};
            const Address peer {t_sock.get_peername()};
            const Address self {t_sock.get_sockname()};
            // clang-format off
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
                 << self
                 << std::endl
                 << std::right << std::setw(indent_width) << "to "
                 << peer
                 << std::endl;
            // clang-format on
            assert(host.text() == peer.text());
        }

    private:
        EndpointView m_endpoint;
        HostnameView m_hostname;
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
            !operands.empty() ? operands[0] : remotehost,
            operands.size() > 1 ? operands[1] : service
        };
    }

    auto test_connect(const EndpointView& endpoint,
                      const SocketHints& hints,
                      const Hostname& hostname) -> void
    {
        const auto connect_result {connect(endpoint, hints, is_verbose)};
        std::ranges::for_each(connect_result,
                              Test(endpoint,
                                   hostname,
                                   std::cout));
    }

    auto test_connect_valid(const EndpointView& endpoint,
                            const SocketHints& hints,
                            const Hostname& hostname) -> void
    {
        test_connect(endpoint, hints, hostname);
    }
}

auto main(int argc, char* argv[]) -> int
{
    try {
        const auto endpoint {parse_arguments(argc, argv)};
        const auto rt {run(is_verbose)};

        if (is_verbose) {
            std::cout << *rt << std::endl;
        }

        const auto hostname {get_hostname(rt)};

        if (getenv("http_proxy") == nullptr) {
            test_connect_valid(endpoint, socket_hints, hostname);
        }
    }
    catch (const Error& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
