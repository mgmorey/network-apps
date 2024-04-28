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
                                        // OsErrorResult, SocketHints,
                                        // SocketHost, get_endpoint(),
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
                            // EAI_FAMILY, EAI_NODATA, EAI_NONAME,
                            // EAI_SOCKTYPE
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
#include <type_traits>  // std::decay_t, std::is_same_v
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
    using Network::IpSocketHints;
    using Network::OptionalHints;
    using Network::OptionalHostname;
    using Network::OsErrorResult;
    using Network::SocketHints;
    using Network::SocketHost;
    using Network::af_ip_v4;
    using Network::af_ip_v6;
    using Network::always_false_v;
    using Network::get_hosts;
    using Network::os_error_type;
    using Network::parse;
    using Network::uniquify;

    using ErrorCodeSet = std::set<os_error_type>;
    using HintsVector = std::vector<SocketHints>;

    static const IpSocketHints ip {SOCK_STREAM, AI_CANONNAME};
    static const IpSocketHints ipv4 {af_ip_v4, SOCK_STREAM, AI_CANONNAME};
    static const IpSocketHints ipv6 {af_ip_v6, SOCK_STREAM, AI_CANONNAME};

    static bool verbose {false};  // NOLINT

    class Print
    {
    public:
        using Values = std::vector<std::string>;

        static auto get_endpoint(const ByteString& addr) -> Endpoint
        {
            return Network::get_endpoint(addr, 0, verbose);
        }

        explicit Print(std::ostream& t_os) :
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

    auto get_codes_family() -> const ErrorCodeSet&
    {
#if defined(WIN32)
        static const ErrorCodeSet codes = {
            WSAEAFNOSUPPORT
        };
#else
        static const ErrorCodeSet codes = {
            EAI_FAMILY
        };
#endif
        return codes;
    }

    auto get_codes_no_data() -> const ErrorCodeSet&
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

    auto get_codes_no_name() -> const ErrorCodeSet&
    {
#if defined(WIN32)
        static const ErrorCodeSet codes = {
            WSAHOST_NOT_FOUND
        };
#else
        static const ErrorCodeSet codes = {
            EAI_NONAME
        };
#endif
        return codes;
    }

    auto get_codes_socktype() -> const ErrorCodeSet&
    {
#if defined(WIN32)
        static const ErrorCodeSet codes = {
            WSAESOCKTNOSUPPORT
        };
#elif defined(OS_DARWIN)
        static const ErrorCodeSet codes = {
            EAI_BADHINTS
        };
#else
        static const ErrorCodeSet codes = {
            EAI_SOCKTYPE
        };
#endif
        return codes;
    }

    auto get_family(const OptionalHints& hints) -> std::string
    {
        if (!hints) {
            return {};
        }

        switch (hints->m_family) {
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
        return is_local ? HintsVector {ip} : HintsVector {ipv4, ipv6};
    }

    auto get_hostname() -> OptionalHostname
    {
        const auto* const hostname {std::getenv("HOSTNAME")};

        if (hostname == nullptr) {
            return {};
        }

        return hostname;
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

    auto print(const OsErrorResult& result) -> void
    {
        if (verbose) {
            std::cout << "Number: "
                      << result.number()
                      << std::endl
                      << "String: "
                      << result.string()
                      << std::endl;
        }
    }

    auto print(const OsErrorResult& result, const std::string& family) -> void
    {
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

    auto print(const HostVector& hosts, const std::string& family) -> void
    {
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
                      Print(std::cout));
    }

    auto test_host(const OptionalHostname& host,
                   const OptionalHints& hints,
                   const ErrorCodeSet& expected_codes = {0}) -> void
    {
        os_error_type actual_code {0};
        const auto family {get_family(hints)};
        const auto hosts_result {get_hosts(host, hints, verbose)};
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, HostVector>) {
                print(arg, family);
            }
            else if constexpr (std::is_same_v<T, OsErrorResult>) {
                if (expected_codes == ErrorCodeSet {0}) {
                    print(arg, family);
                }
                else {
                    print(arg);
                }

                actual_code = arg.number();
            }
            else {
                static_assert(always_false_v<T>, VISITOR_ERROR);
            }
        }, hosts_result);

        assert(expected_codes.contains(actual_code));
    }

    auto test_invalid_family() -> void
    {
        const SocketHints hints {-1, SOCK_STREAM, 0, 0};
        test_host("localhost", hints, get_codes_family());
    }

    auto test_invalid_socktype() -> void
    {
        const SocketHints hints {AF_UNSPEC, -1, 0, 0};
        test_host("localhost", hints, get_codes_socktype());
    }

    auto test_no_data() -> void
    {
        const IpSocketHints hints {SOCK_STREAM};
        test_host(".", hints, get_codes_no_data());
    }

    auto test_no_name() -> void
    {
        const IpSocketHints hints {SOCK_STREAM};
        test_host("_", hints, get_codes_no_name());
    }

    auto test_valid(const OptionalHostname& host) -> void
    {
        const bool is_local = !host || *host == get_hostname();

        if (host && !is_local) {
            std::cout << "Host: "
                      << *host
                      << std::endl;
        }

        const auto& hints_vector {get_hints_vector(is_local)};

        for (const auto& hints : hints_vector) {
            test_host(host, hints, {0});
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
            std::for_each(hosts.begin(), hosts.end(), test_valid);
        }
        else if (const auto hostname {get_hostname()}) {
            test_valid(hostname);
        }
        else {
            test_valid("localhost");
        }

        test_invalid_family();
        test_invalid_socktype();
        test_no_data();
        test_no_name();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
