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

#include "network/network.h"            // Address, Bytes, Context,
                                        // Endpoint, EndpointResult,
                                        // Hints, Host, HostVector,
                                        // Hostname, OptionalHints,
                                        // OsErrorResult, Overloaded,
                                        // get_endpoint(),
                                        // get_hosts(), skip_first(),
                                        // uniquify()
#include "network/assert.h"             // assert()

#ifdef WIN32
#include <getopt.h>         // getopt(), optarg, opterr, optind
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
#include <unistd.h>         // getopt(), optarg, opterr, optind
#endif

#include <algorithm>    // std::for_each(), std::remove()
#include <cstdlib>      // EXIT_FAILURE, std::exit(), std::getenv()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <iterator>     // std::next()
#include <ostream>      // std::ostream
#include <set>          // std::set
#include <span>         // std::span
#include <string>       // std::string
#include <utility>      // std::pair
#include <variant>      // std::visit()
#include <vector>       // std::vector

using Network::Address;
using Network::ByteString;
using Network::Context;
using Network::Endpoint;
using Network::EndpointResult;
using Network::Hints;
using Network::Host;
using Network::HostVector;
using Network::OptionalHints;
using Network::OptionalHostname;
using Network::OsErrorResult;
using Network::Overloaded;
using Network::get_endpoint;
using Network::get_hosts;
using Network::os_error_type;
using Network::skip_first;
using Network::uniquify;

namespace TestHost
{
    using ErrorCodeSet = std::set<os_error_type>;
    using HintsVector = std::vector<Hints>;

    static bool verbose {false};  // NOLINT

    class Test
    {
    public:
        using Values = std::vector<std::string>;

        explicit Test(std::ostream& t_os) :
            m_os(t_os)
        {
        }

        auto operator()(const Host& t_host) -> void
        {
            const ByteString addr {t_host.address()};
            const auto endpoint_result {get_endpoint(addr)};
            std::visit(Overloaded {
                    [&](const Endpoint& endpoint) {
                        Values values = {
                            static_cast<Address>(addr).text(),
                            endpoint.first.value_or(""),
                            t_host.canonical_name().value_or("")
                        };
                        values.erase(std::remove(values.begin(),
                                                 values.end(),
                                                 ""),
                                     values.end());
                        uniquify(values);
                        print(values);
                    },
                    [&](const OsErrorResult& result) {
                        std::cerr << result.string()
                                  << std::endl;
                    }
                }, endpoint_result);
        }

        static auto get_endpoint(const ByteString& addr) -> EndpointResult
        {
            return Network::get_endpoint(addr, 0, verbose);
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

    static auto get_family(const OptionalHints& hints) -> std::string
    {

        switch (hints ? hints->family() : 0) {
        case AF_INET:
            return "IPv4";
        case AF_INET6:
            return "IPv6";
        default:
            return "";
        }
    }

    static auto get_hints_vector(bool is_local) -> const std::vector<Hints>&
    {
        static constexpr Hints inetHints {AI_CANONNAME, AF_INET};
        static constexpr Hints inet6Hints {AI_CANONNAME, AF_INET6};
        static constexpr Hints unspecHints {AI_CANONNAME};
        static const HintsVector inetHintsVector {inetHints, inet6Hints};
        static const HintsVector unspecHintsVector {unspecHints};
        return is_local ? unspecHintsVector : inetHintsVector;
    }

    static auto get_hostname() -> OptionalHostname
    {
        const char* hostname_c {std::getenv("HOSTNAME")};
        OptionalHostname hostname;

        if (hostname_c != nullptr) {
            hostname = hostname_c;
        }

        return hostname;
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

    static auto test_host(const OptionalHostname& host,
                          const OptionalHints& hints = {}) -> void
    {
        auto hosts_result {get_hosts(host, hints)};
        const auto family {get_family(hints)};
        std::visit(Overloaded {
                [&](HostVector& hosts) {
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

    static auto test_host_invalid() -> void
    {
        os_error_type actual_code {0};
        const auto& expected_code {get_code_nodata()};
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
        assert(expected_code.count(actual_code) != 0);
    }

    static auto test_host_valid(const OptionalHostname& host) -> void
    {
        bool is_local = !host || *host == get_hostname();

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
        const auto args {parse_arguments(argc, argv)};
        const auto& context {Context::instance()};

        if (verbose) {
            std::cerr << context;
        }

        test_host_invalid();

        if (args.size() > 1) {
            for (const auto& host : skip_first(args)) {
                test_host_valid(host);
            }
        }
        else {
            test_host_valid(get_hostname());
        }
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
