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
#include "network/network.h"            // Address, Bytes,
                                        // ByteStringResult, Context,
                                        // Endpoint, FdResult,
                                        // FdResultVector, Hints,
                                        // OptionalHostname,
                                        // OsErrorResult, Overloaded,
                                        // close(), connect(),
                                        // get_hostname(),
                                        // get_peername(),
                                        // get_sockname(), string_null
#include "network/os-features.h"        // WIN32

#ifdef WIN32
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
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iomanip>      // std::right, std::setw()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <set>          // std::set
#include <span>         // std::span
#include <string>       // std::string
#include <utility>      // std::move()
#include <variant>      // std::get(), std::holds_alternative,
                        // std::visit()
#include <vector>       // std::vector

namespace TestConnect
{
    using ErrorCodeSet = std::set<Network::os_error_type>;

    constexpr auto fd_width {6};
    constexpr auto indent_width {fd_width + 18};
    constexpr auto localhost {"example.com"};
    constexpr auto localservice {"http"};

    static bool verbose {false};  // NOLINT

    class Test
    {
    public:
        Test(Network::Endpoint t_endpoint,
             Network::OptionalHostname t_hostname,
             std::ostream& t_os) :
            m_endpoint(std::move(t_endpoint)),
            m_hostname(std::move(t_hostname)),
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

        static auto get_peeraddr(const Network::Fd& t_fd) ->
            Network::ByteStringResult
        {
            auto peername_result {Network::get_peername(t_fd, verbose)};
            std::visit(Network::Overloaded {
                    [&](const Network::Bytes& addr) {
                        static_cast<void>(addr);
                    },
                    [&](const Network::OsErrorResult& result) {
                        std::cerr << result.string()
                                  << std::endl;
                    }
                }, peername_result);
            return peername_result;
        }

        static auto get_sockaddr(const Network::Fd& t_fd) ->
            Network::ByteStringResult
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
            const auto peer_result {get_peeraddr(t_fd)};
            const auto sock_result {get_sockaddr(t_fd)};
            m_os << "Socket "
                 << std::right << std::setw(fd_width) << t_fd
                 << " connected "
                 << m_hostname.value_or(Network::string_null)
                 << " to "
                 << service.value_or(Network::string_null)
                 << " on "
                 << hostname.value_or(Network::string_null)
                 << std::endl;

            if (std::holds_alternative<Network::Bytes>(peer_result) &&
                std::holds_alternative<Network::Bytes>(sock_result)) {
                const auto& peer {std::get<Network::Bytes>(peer_result)};
                const auto& self {std::get<Network::Bytes>(sock_result)};
                m_os << "Socket "
                     << std::right << std::setw(fd_width) << t_fd
                     << " connected "
                     << Network::Address(self)
                     << std::endl
                     << std::right << std::setw(indent_width) << "to "
                     << Network::Address(peer)
                     << std::endl;
            }

            Network::close(t_fd);
            m_os << "Socket "
                 << std::right << std::setw(fd_width) << t_fd
                 << " closed"
                 << std::endl;
        }

    private:
        Network::Endpoint m_endpoint;
        Network::OptionalHostname m_hostname;
        std::ostream& m_os;
    };

    static auto get_code_nodata() -> const ErrorCodeSet&
    {
#if defined(WIN32)
        static const ErrorCodeSet expected_code_nodata = {
            WSAHOST_NOT_FOUND
        };
#elif defined(OS_FREEBSD)
        static const ErrorCodeSet expected_code_nodata = {
            EAI_AGAIN,
            EAI_NONAME
        };
#else
        static const ErrorCodeSet expected_code_nodata = {
            EAI_AGAIN,
            EAI_NODATA,
            EAI_NONAME
        };
#endif
        return expected_code_nodata;
    }

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

    static auto print(const Network::OsErrorResult& result,
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

    static auto test_connect(const Network::Endpoint& endpoint,
                             const Network::Hints& hints,
                             const Network::Hostname& hostname) -> void
    {
        const auto open_result {
            Network::connect(endpoint, hints, verbose)
        };
        std::visit(Network::Overloaded {
                [&](const Network::FdResultVector& fd_results) {
                    std::for_each(fd_results.begin(),
                                  fd_results.end(),
                                  Test(endpoint,
                                       hostname,
                                       std::cout));
                },
                [&](const Network::ErrorResult& result) {
                    std::cerr << result.string()
                              << std::endl;
                }
            }, open_result);
    }

    static auto test_connect_invalid(const Network::Endpoint& endpoint,
                                     const Network::Hints& hints) -> void
    {
        Network::os_error_type actual_code {0};
        const auto& expected_code {get_code_nodata()};
        const auto open_result {
            Network::connect(endpoint, hints, verbose)
        };
        std::visit(Network::Overloaded {
                [&](const Network::FdResultVector& fd_results) {
                    static_cast<void>(fd_results);
                },
                [&](const Network::ErrorResult& result) {
                    print(result, "connect() with invalid endpoint");
                    actual_code = result.number();
                }
            }, open_result);
        assert(expected_code.count(actual_code) != 0);
    }

    static auto test_connect_valid(const Network::Endpoint& endpoint,
                                   const Network::Hints& hints) -> void
    {
        const auto hostname_result {Network::get_hostname()};
        std::visit(Network::Overloaded {
                [&](const std::string& hostname) {
                    test_connect(endpoint, hints, hostname);
                },
                [&](const Network::OsErrorResult& result) {
                    std::cerr << "No hostname available: "
                              << result.string()
                              << std::endl;
                }
            }, hostname_result);
    }
}

auto main(int argc, char* argv[]) -> int
{
    static constexpr Network::Hints hints
        {AI_CANONNAME, AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP};

    try {
        const auto args {TestConnect::parse_arguments(argc, argv)};
        const auto& context {Network::Context::instance()};

        if (TestConnect::verbose) {
            std::cerr << context;
        }

        const Network::Endpoint invalid_endpoint {
            ".",
            TestConnect::localservice
        };
        TestConnect::test_connect_invalid(invalid_endpoint, hints);
        const Network::Endpoint valid_endpoint {
            args.size() > 1 ? args[1] : TestConnect::localhost,
            args.size() > 2 ? args[2] : TestConnect::localservice
        };
        TestConnect::test_connect_valid(valid_endpoint, hints);
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
