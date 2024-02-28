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

#include "network/argumentspan.h"       // ArgumentSpan
#include "network/assert.h"             // assert()
#include "network/network.h"            // Address, ByteString,
                                        // Context, Endpoint,
                                        // HostVector, OptionalHints,
                                        // OptionalHostname,
                                        // OsErrorResult, Overloaded,
                                        // SocketHints, SocketHost,
                                        // get_endpoint(),
                                        // get_hosts(), os_error_type,
                                        // uniquify()
#include "network/parse.h"              // parse()

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_UNSPEC,
                            // IPPROTO_TCP, SOCK_STREAM,
                            // WSAHOST_NOT_FOUND
#include <ws2tcpip.h>       // AI_ADDRCONFIG, AI_CANONNAME
#else
#include <netdb.h>          // AI_ADDRCONFIG, AI_CANONNAME, EAI_AGAIN,
                            // EAI_NODATA, EAI_NONAME
#include <netinet/in.h>     // IPPROTO_TCP
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_UNSPEC,
                            // SOCK_STREAM
#endif

#include <algorithm>    // std::for_each(), std::remove()
#include <cstdlib>      // EXIT_FAILURE, std::exit(), std::getenv(),
                        // std::size_t
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <iterator>     // std::next()
#include <ostream>      // std::ostream
#include <set>          // std::set
#include <string>       // std::string
#include <utility>      // std::pair
#include <variant>      // std::visit()
#include <vector>       // std::vector

namespace TestHost
{
    using Network::Address;
    using Network::ArgumentSpan;
    using Network::ByteString;
    using Network::Context;
    using Network::Endpoint;
    using Network::HostVector;
    using Network::OptionalHints;
    using Network::OptionalHostname;
    using Network::OsErrorResult;
    using Network::Overloaded;
    using Network::SocketHints;
    using Network::SocketHost;
    using Network::get_hosts;
    using Network::os_error_type;
    using Network::parse;
    using Network::uniquify;

    using ErrorCodeSet = std::set<os_error_type>;
    using HintsVector = std::vector<SocketHints>;

    static bool verbose {false};  // NOLINT

    static constexpr SocketHints inet {
        AI_CANONNAME, AF_INET, SOCK_STREAM
    };
    static constexpr SocketHints inet6 {
        AI_CANONNAME, AF_INET6, SOCK_STREAM
    };
    static constexpr SocketHints unspec {
        AI_CANONNAME, AF_UNSPEC, SOCK_STREAM
    };

    class Test
    {
    public:
        using Values = std::vector<std::string>;

        static auto get_endpoint(const ByteString& addr) -> Endpoint
        {
            return Network::get_endpoint(addr, 0, verbose);
        }

        explicit Test(std::ostream& t_os) :
            m_os(t_os)
        {
        }

        auto operator()(const SocketHost& t_host) -> void
        {
            const ByteString& addr {t_host.address()};
            const auto endpoint {get_endpoint(addr)};
            Values values = {
                Address(addr).text(),
                endpoint.first.value_or(std::string {}),
                t_host.canonical_name().value_or(std::string {})
            };
            values.erase(std::remove(values.begin(),
                                     values.end(),
                                     std::string {}),
                         values.end());
            uniquify(values);
            print(values);
        }

        auto print(const Values& values) -> void
        {
            if (values.empty()) {
                return;
            }

            m_os << "    ";

            for (std::size_t i = 0; i < values.size(); ++i) {
                if (i > 1) {
                    m_os << ", ";
                }

                m_os << values[i];

                if (i == 0U) {
                    m_os << " (";
                }
            }

            m_os << ')'
                 << std::endl;
        }

    private:
        std::ostream& m_os;
    };

    auto get_codes_nodata() -> const ErrorCodeSet&
    {
#if defined(WIN32)
        static const ErrorCodeSet codes = {
            WSAHOST_NOT_FOUND
        };
#elif defined(OS_FREEBSD)
        static const ErrorCodeSet codes = {
            EAI_AGAIN,
            EAI_NONAME
        };
#else
        static const ErrorCodeSet codes = {
            EAI_AGAIN,
            EAI_NODATA,
            EAI_NONAME
        };
#endif
        return codes;
    }

    auto get_family(const OptionalHints& hints) -> std::string
    {
        if (!hints) {
            return {};
        }

        switch (hints->family()) {
        case AF_INET:
            return "IPv4";
        case AF_INET6:
            return "IPv6";
        default:
            return {};
        }
    }

    auto get_hints_vector(bool is_local) -> HintsVector
    {
        return is_local ? HintsVector {unspec} : HintsVector {inet, inet6};
    }

    auto get_hostname() -> OptionalHostname
    {
        const auto* const hostname_c {std::getenv("HOSTNAME")};

        if (hostname_c == nullptr) {
            return {};
        }

        return hostname_c;
    }

    auto parse_arguments(int argc, char** argv) -> ArgumentSpan
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

        return operands;
    }

    auto print(const OsErrorResult& result,
               const std::string& description = {}) -> void
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

    auto test_host(const OptionalHostname& host,
                   const OptionalHints& hints) -> void
    {
        const auto hosts_result {get_hosts(host, hints)};
        const auto family {get_family(hints)};
        std::visit(Overloaded {
                [&](const HostVector& hosts) {
                    if (hosts.empty()) {
                        return;
                    }

                    if (family.empty()) {
                        std::cout << "All";
                    }
                    else {
                        std::cout << family;
                    }

                    std::cout << " hosts:"
                              << std::endl;
                    std::for_each(hosts.begin(), hosts.end(),
                                  Test(std::cout));
                },
                    [&](const OsErrorResult& result) {
                        if (family.empty()) {
                            std::cout << "No";
                        }
                        else {
                            std::cout << "No "
                                      << family;
                        }

                        std::cout << " hosts:"
                                  << std::endl
                                  << result.string()
                                  << std::endl;
                    }
                    }, hosts_result);
    }

    auto test_host_invalid() -> void
    {
        os_error_type actual_code {0};
        const auto& expected_codes {get_codes_nodata()};
        const auto hosts_result {get_hosts(".")};
        std::visit(Overloaded {
                [&](const HostVector& hosts) {
                    static_cast<void>(hosts);
                },
                    [&](const OsErrorResult& result) {
                        print(result, "get_hosts() with invalid hostname");
                        actual_code = result.number();
                    }
                    }, hosts_result);
        assert(expected_codes.contains(actual_code));
    }

    auto test_host_valid(const OptionalHostname& host) -> void
    {
        const bool is_local = !host || *host == get_hostname();

        if (host && !is_local) {
            std::cout << "Host: "
                      << *host
                      << std::endl;
        }

        const auto& hints_vector {get_hints_vector(is_local)};

        for (const auto& hints : hints_vector) {
            test_host(host, hints);
        }
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace TestHost;

    try {
        const auto& context {Context::instance()};
        const auto hosts {parse_arguments(argc, argv)};

        if (verbose) {
            std::cout << context << std::endl;
        }

        if (!hosts.empty()) {
            std::for_each(hosts.begin(), hosts.end(), test_host_valid);
        }
        else if (const auto hostname {get_hostname()}) {
            test_host_valid(hostname);
        }

        test_host_invalid();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
