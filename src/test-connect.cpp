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
#include <cstdlib>      // EXIT_FAILURE, std::exit(), std::size_t
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
using Network::Context;
using Network::Endpoint;
using Network::Fd;
using Network::FdResult;
using Network::FdResultVector;
using Network::Hostname;
using Network::OptionalHostname;
using Network::OsErrorResult;
using Network::Overloaded;
using Network::PeerName;
using Network::SocketHints;
using Network::SockName;
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

        auto test_socket(const Fd& t_fd) -> void
        {
            const auto hostname {m_endpoint.first};
            const auto service {m_endpoint.second};
            const auto peer {get_peername(t_fd, verbose)};
            const auto self {get_sockname(t_fd, verbose)};
            m_os << "Socket "
                 << std::right << std::setw(fd_width) << t_fd
                 << " connected "
                 << m_hostname.value_or(string_null)
                 << " to "
                 << service.value_or(string_null)
                 << " on "
                 << hostname.value_or(string_null)
                 << std::endl
                 << "Socket "
                 << std::right << std::setw(fd_width) << t_fd
                 << " connected "
                 << Address(self)
                 << std::endl
                 << std::right << std::setw(indent_width) << "to "
                 << Address(peer)
                 << std::endl;
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

    static auto get_codes_invalid_addr() -> const ErrorCodeSet&
    {
#if defined(WIN32)
        static const ErrorCodeSet codes {WSAEFAULT};
#else
        static const ErrorCodeSet codes {EAFNOSUPPORT};
#endif
        return codes;
    }

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

        const auto args = std::span(argv, std::size_t(argc));

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

    static auto test_connect_invalid_addr(const ByteString& addr,
                                          const SocketHints& hints) -> void
    {
        os_error_type actual_code {0};
        const auto& expected_codes {get_codes_invalid_addr()};
        const auto fd_result {get_socket(hints, verbose)};
        std::visit(Overloaded {
                [&](const Fd& fd) {
                    const auto error {Network::connect(fd, addr, verbose)};
                    actual_code = error.number();

                    if (error) {
                        print(error, "connect() with invalid address");
                    }

                    Network::close(fd);
                },
                [&](const OsErrorResult& error) {
                    actual_code = error.number();
                    print(error, "connect() with invalid address");
                }
            }, fd_result);
        assert(expected_codes.count(actual_code) != 0);
    }

    static auto test_connect_invalid_host(const Endpoint& endpoint,
                                          const SocketHints& hints) -> void
    {
        os_error_type actual_code {0};
        const auto& expected_codes {get_codes_invalid_host()};
        const auto connect_result {Network::connect(endpoint, hints, verbose)};
        std::visit(Overloaded {
                [&](const FdResultVector& fd_results) {
                    static_cast<void>(fd_results);
                },
                [&](const OsErrorResult& error) {
                    actual_code = error.number();
                    print(error, "connect() with invalid host");
                }
            }, connect_result);
        assert(expected_codes.count(actual_code) != 0);
    }

    static auto test_connect_invalid_service(const Endpoint& endpoint,
                                             const SocketHints& hints) -> void
    {
        os_error_type actual_code {0};
        const auto& expected_codes {get_codes_invalid_service()};
        const auto connect_result {Network::connect(endpoint, hints, verbose)};
        std::visit(Overloaded {
                [&](const FdResultVector& fd_results) {
                    static_cast<void>(fd_results);
                },
                [&](const OsErrorResult& error) {
                    actual_code = error.number();
                    print(error, "connect() with invalid service");
                }
            }, connect_result);
        assert(expected_codes.count(actual_code) != 0);
    }

    static auto test_connect_valid(const Endpoint& endpoint,
                                   const SocketHints& hints,
                                   const Hostname& hostname) -> void
    {
        const auto connect_result {Network::connect(endpoint, hints, verbose)};
        std::visit(Overloaded {
                [&](const FdResultVector& fd_results) {
                    std::for_each(fd_results.begin(),
                                  fd_results.end(),
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

    static constexpr SocketHints inet_hints
        {AI_CANONNAME, AF_INET, SOCK_STREAM, IPPROTO_TCP};

    static constexpr SocketHints unspec_hints
        {AI_CANONNAME, AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP};

    try {
        const auto args {parse_arguments(argc, argv)};
        const auto& context {Context::instance()};

        if (verbose) {
            std::cerr << context;
        }

        const ByteString invalid_addr {};
        test_connect_invalid_addr(invalid_addr, inet_hints);
        const Endpoint invalid_host {".", localservice};
        test_connect_invalid_host(invalid_host, unspec_hints);
        const Endpoint invalid_service {localhost, "."};
        test_connect_invalid_service(invalid_service, unspec_hints);
        const Endpoint valid_endpoint {
            args.size() > 1 ? args[1] : localhost,
            args.size() > 2 ? args[2] : localservice
        };
        test_connect_valid(valid_endpoint, unspec_hints);
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
