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

#include "network/network.h"            // Address, Bytes,
                                        // BytesResult, Endpoint,
                                        // FdResult, Hints,
                                        // OsErrorResult, Overloaded,
                                        // bind(), close(),
                                        // get_sockname()

#ifdef _WIN32
#include <getopt.h>         // getopt(), optarg, opterr, optind
#include <winsock2.h>       // AF_INET, AF_INET6, PF_INET, PF_INET6,
                            // IPPROTO_IP, IPPROTO_TCP, IPPROTO_UDP,
                            // SOCK_DGRAM, SOCK_STREAM
#include <ws2tcpip.h>       // AI_ADDRCONFIG, AI_ALL, AI_CANONNAME
#else
#include <netdb.h>          // AI_ADDRCONFIG, AI_ALL, AI_CANONNAME
#include <netinet/in.h>     // IPPROTO_IP, IPPROTO_TCP, IPPROTO_UDP
#include <sys/socket.h>     // AF_INET, AF_INET6, PF_INET, PF_INET6,
                            // SOCK_DGRAM, SOCK_STREAM
#include <unistd.h>         // getopt(), optarg, opterr, optind
#endif

#include <algorithm>    // std::for_each()
#include <cassert>      // assert()
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <span>         // std::span
#include <string>       // std::string
#include <variant>      // std::get(), std::holds_alternative(),
                        // std::visit()
#include <utility>      // std::move()
#include <vector>       // std::vector

namespace TestBind
{
    constexpr auto localhost {"localhost"};
    constexpr auto localservice {"8085"};

    static bool verbose {false};  // NOLINT

    class Test
    {
    public:
        Test(Network::Endpoint t_endpoint, std::ostream& t_os) :
            m_endpoint(std::move(t_endpoint)),
            m_os(t_os)
        {
        }

        auto operator()(const Network::FdResult& t_fd_result) -> void
        {
            std::visit(Network::Overloaded {
                    [&](Network::Fd fd) {
                        test_socket(fd);
                    },
                    [&](const Network::OsErrorResult& result) {
                        std::cerr << result.string()
                                  << std::endl;
                    }
                }, t_fd_result);
        }

        static auto get_sockaddr(const Network::Fd& t_fd) ->
            Network::BytesResult
        {
            auto sockname_result {Network::get_sockname(t_fd, verbose)};
            std::visit(Network::Overloaded {
                    [&](const Network::Bytes& addr) {
                        static_cast<void>(addr);
                    },
                    [&](const Network::OsErrorResult& result) {
                        std::cerr << result.string()
                                  << std::endl;
                    }
                }, sockname_result);
            return sockname_result;
        }

        auto test_socket(const Network::Fd& t_fd) -> void
        {
            const auto hostname {m_endpoint.first};
            const auto service {m_endpoint.second};
            const auto sock_result {get_sockaddr(t_fd)};
            m_os << "Socket "
                 << t_fd
                 << " bound to "
                 << service.value_or(Network::string_null)
                 << " on "
                 << hostname.value_or(Network::string_null)
                 << std::endl;

            if (std::holds_alternative<Network::Bytes>(sock_result)) {
                const auto& self {std::get<Network::Bytes>(sock_result)};
                m_os << "Socket "
                     << t_fd
                     << " bound to "
                     << Network::Address(self)
                     << std::endl;
            }

            Network::close(t_fd);
            m_os << "Socket "
                 << t_fd
                 << " closed"
                 << std::endl;
        }

    private:
        Network::Endpoint m_endpoint;
        std::ostream& m_os;
    };

    static auto parse_arguments(int argc, char** argv) ->
        std::vector<std::string>
    {
        std::vector<std::string> result {*argv};
        int ch {};

        while ((ch = ::getopt(argc, argv, "v")) != -1) {
            switch (ch) {
            case 'v':
                verbose = true;
                break;
            case '?':
                std::cerr << "Usage: "
                          << *argv
                          << " [-v]"
                          << std::endl;
                std::exit(EXIT_FAILURE);
            default:
                abort();
            }
        }

        const auto args = std::span(argv, size_t(argc));

        for (auto index = optind; index < argc; ++index) {
            result.emplace_back(args[index]);
        }

        return result;
    }

    static auto test_bind(const Network::Endpoint& endpoint,
                          const Network::Hints& hints) -> void
    {
        const auto open_result {Network::bind(endpoint, hints, verbose)};
        std::visit(Network::Overloaded {
                [&](const Network::FdResultVector& fd_results) {
                    assert(!fd_results.empty());  // NOLINT
                    std::for_each(fd_results.begin(), fd_results.end(),
                                  Test(endpoint, std::cout));
                },
                [&](const Network::ErrorResult& result) {
                    std::cerr << result.string()
                              << std::endl;
                }
            }, open_result);
    }
}

auto main(int argc, char* argv[]) -> int
{
    static const Network::Hints hints
        {AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP, AI_CANONNAME};

    try {
        const auto args {TestBind::parse_arguments(argc, argv)};
        const Network::Context context {TestBind::verbose};

        if (context.result()) {
            std::cerr << context.result().string()
                      << std::endl;
        }
        else {
            const Network::Endpoint endpoint {
                args.size() > 1 ? args[1] : TestBind::localhost,
                args.size() > 2 ? args[2] : TestBind::localservice
            };
            TestBind::test_bind(endpoint, hints);
        }

        static_cast<void>(context);
    }
    catch (std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
