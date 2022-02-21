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
                                        // FdResultVector,
                                        // OptionalHostname,
                                        // OsErrorResult, Overloaded,
                                        // SocketHints, close(),
                                        // connect(), get_hostname(),
                                        // get_peername(),
                                        // get_sockname(), string_null

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

using Network::Address;
using Network::ByteString;
using Network::ByteStringResult;
using Network::Context;
using Network::Endpoint;
using Network::Fd;
using Network::FdResult;
using Network::FdResultVector;
using Network::Hostname;
using Network::OptionalHostname;
using Network::OsErrorResult;
using Network::Overloaded;
using Network::SocketHints;
using Network::connect;
using Network::get_hostname;
using Network::get_peername;
using Network::get_sockname;
using Network::os_error_type;
using Network::string_null;

namespace TestConnect
{
    using ErrorCodeSet = std::set<os_error_type>;

    constexpr auto fd_width {6};
    constexpr auto indent_width {fd_width + 18};
    constexpr auto localhost {"example.com"};
    constexpr auto localservice {"http"};

    static bool verbose {false};  // NOLINT

    class Test
    {
    public:
        Test(Endpoint t_endpoint,
             OptionalHostname t_hostname,
             std::ostream& t_os) :
            m_endpoint(std::move(t_endpoint)),
            m_hostname(std::move(t_hostname)),
            m_os(t_os)
        {
        }

        auto operator()(const FdResult& t_fd_result) -> void
        {
            std::visit(Overloaded {
                    [&](Fd fd) {
                        test_socket(fd);
                    },
                    [&](const OsErrorResult& result) {
                        std::cerr << result.string()
                                  << std::endl;
                    }
                }, t_fd_result);
        }

        static auto get_peeraddr(const Fd& t_fd) -> ByteStringResult
        {
            auto peername_result {get_peername(t_fd, verbose)};
            std::visit(Overloaded {
                    [&](const ByteString& addr) {
                        static_cast<void>(addr);
                    },
                    [&](const OsErrorResult& result) {
                        std::cerr << result.string()
                                  << std::endl;
                    }
                }, peername_result);
            return peername_result;
        }

        static auto get_sockaddr(const Fd& t_fd) -> ByteStringResult
        {
            auto sockname_result {get_sockname(t_fd, verbose)};
            std::visit(Overloaded {
                    [&](const ByteString& addr) {
                        static_cast<void>(addr);
                    },
                    [&](const OsErrorResult& result) {
                        std::cerr << result.string()
                                  << std::endl;
                    }
                }, sockname_result);
            return sockname_result;
        }

        auto test_socket(const Fd& t_fd) -> void
        {
            const auto hostname {m_endpoint.first};
            const auto service {m_endpoint.second};
            const auto peer_result {get_peeraddr(t_fd)};
            const auto sock_result {get_sockaddr(t_fd)};
            m_os << "Socket "
                 << std::right << std::setw(fd_width) << t_fd
                 << " connected "
                 << m_hostname.value_or(string_null)
                 << " to "
                 << service.value_or(string_null)
                 << " on "
                 << hostname.value_or(string_null)
                 << std::endl;

            if (std::holds_alternative<ByteString>(peer_result) &&
                std::holds_alternative<ByteString>(sock_result)) {
                const auto& peer {std::get<ByteString>(peer_result)};
                const auto& self {std::get<ByteString>(sock_result)};
                m_os << "Socket "
                     << std::right << std::setw(fd_width) << t_fd
                     << " connected "
                     << Address(self)
                     << std::endl
                     << std::right << std::setw(indent_width) << "to "
                     << Address(peer)
                     << std::endl;
            }

            Network::close(t_fd, verbose);
            m_os << "Socket "
                 << std::right << std::setw(fd_width) << t_fd
                 << " closed"
                 << std::endl;
        }

    private:
        Endpoint m_endpoint;
        OptionalHostname m_hostname;
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

    static auto test_connect(const Endpoint& endpoint,
                             const SocketHints& hints,
                             const Hostname& hostname) -> void
    {
        const auto open_result {connect(endpoint, hints, verbose)};
        std::visit(Overloaded {
                [&](const FdResultVector& fd_results) {
                    std::for_each(fd_results.begin(),
                                  fd_results.end(),
                                  Test(endpoint,
                                       hostname,
                                       std::cout));
                },
                [&](const OsErrorResult& result) {
                    std::cerr << result.string()
                              << std::endl;
                }
            }, open_result);
    }

    static auto test_connect_invalid(const Endpoint& endpoint,
                                     const SocketHints& hints) -> void
    {
        os_error_type actual_code {0};
        const auto& expected_code {get_code_nodata()};
        const auto open_result {connect(endpoint, hints, verbose)};
        std::visit(Overloaded {
                [&](const FdResultVector& fd_results) {
                    static_cast<void>(fd_results);
                },
                [&](const OsErrorResult& result) {
                    print(result, "connect() with invalid endpoint");
                    actual_code = result.number();
                }
            }, open_result);
        assert(expected_code.count(actual_code) != 0);
    }

    static auto test_connect_valid(const Endpoint& endpoint,
                                   const SocketHints& hints) -> void
    {
        const auto hostname {get_hostname()};
        test_connect(endpoint, hints, hostname);
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace TestConnect;

    static constexpr SocketHints hints
        {AI_CANONNAME, AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP};

    try {
        const auto args {parse_arguments(argc, argv)};
        const auto& context {Context::instance()};

        if (verbose) {
            std::cerr << context;
        }

        const Endpoint invalid_endpoint {".", localservice};
        test_connect_invalid(invalid_endpoint, hints);
        const Endpoint valid_endpoint {
            args.size() > 1 ? args[1] : localhost,
            args.size() > 2 ? args[2] : localservice
        };
        test_connect_valid(valid_endpoint, hints);
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
