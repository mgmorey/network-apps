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
                                        // Hostname, OsErrorResult,
                                        // Overloaded, get_endpoint(),
                                        // get_hosts(), skip_first(),
                                        // uniquify()
#include "network/assert.h"             // assert()
#include "network/os-features.h"        // WIN32

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

namespace TestHost
{
    using CodeSet = std::set<Network::os_error_type>;

#if defined(WIN32)
    static const CodeSet expected_code_nodata = {
        WSAHOST_NOT_FOUND
    };
#elif defined(OS_FREEBSD)
    static const CodeSet expected_code_nodata = {
        EAI_AGAIN,
        EAI_NONAME
    };
#else
    static const CodeSet expected_code_nodata = {
        EAI_AGAIN,
        EAI_NODATA,
        EAI_NONAME
    };
#endif

    static bool verbose {false};  // NOLINT

    class Test
    {
    public:
        using Values = std::vector<std::string>;

        explicit Test(std::ostream& t_os) :
            m_os(t_os)
        {
        }

        auto operator()(const Network::Host& t_host) -> void
        {
            const Network::Bytes addr {t_host.address()};
            const auto endpoint_result {get_endpoint(addr)};
            std::visit(Network::Overloaded {
                    [&](const Network::Endpoint& endpoint) {
                        Values values = {
                            static_cast<Network::Address>(addr).text(),
                            endpoint.first.value_or(""),
                            t_host.canonical_name().value_or("")
                        };
                        values.erase(std::remove(values.begin(),
                                                 values.end(),
                                                 ""),
                                     values.end());
                        Network::uniquify(values);
                        print(values);
                    },
                    [&](const Network::OsErrorResult& result) {
                        std::cerr << result.string()
                                  << std::endl;
                    }
                }, endpoint_result);
        }

        static auto get_endpoint(const Network::Bytes& addr) ->
            Network::EndpointResult
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

    static auto get_description(const Network::OptionalHints& hints) -> std::string
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

    static auto get_hints_vector(bool named) -> std::vector<Network::Hints>
    {
        static const Network::Hints inet {AI_CANONNAME, AF_INET};
        static const Network::Hints inet6 {AI_CANONNAME, AF_INET6};
        static const Network::Hints unspec {AI_CANONNAME};

        std::vector<Network::Hints> result;

        if (named) {
            result = {inet, inet6};
        }
        else {
            result = {unspec};
        }

        return result;
    }

    static auto get_hostname() -> Network::OptionalHostname
    {
        const char* hostname_c {std::getenv("HOSTNAME")};
        Network::OptionalHostname hostname;

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

    static auto test_host(const Network::OptionalHostname& hostname,
                          const Network::OptionalHints& hints = {}) -> void
    {
        const auto description {get_description(hints)};
        auto hosts_result {Network::get_hosts(hostname, hints)};
        std::visit(Network::Overloaded {
                [&](Network::HostVector& hosts) {
                    if (hosts.empty()) {
                        return;
                    }

                    if (description.empty()) {
                        std::cout << "All";
                    }
                    else {
                        std::cout << description;
                    }

                    std::cout << " hosts:"
                              << std::endl;
                    std::for_each(hosts.begin(), hosts.end(),
                                  Test(std::cout));
                },
                [&](const Network::OsErrorResult& result) {
                    if (description.empty()) {
                        std::cout << "No";
                    }
                    else {
                        std::cout << "No "
                                  << description;
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
        Network::os_error_type actual_code {0};
        const auto hosts_result {Network::get_hosts(".")};
        std::visit(Network::Overloaded {
                [&](const Network::HostVector& hosts) {
                    static_cast<void>(hosts);
                },
                [&](const Network::OsErrorResult& result) {
                    print(result, "get_hosts() with invalid hostname");
                    actual_code = result.number();
                }
            }, hosts_result);
        assert(expected_code_nodata.count(actual_code) != 0);
    }

    static auto test_host_valid(const Network::OptionalHostname& hostname) -> void
    {
        bool is_local = !hostname || *hostname == get_hostname();

        if (hostname && !is_local) {
            std::cout << "Host: "
                      << *hostname
                      << std::endl;
        }

        const auto hints_vector {get_hints_vector(!is_local)};

        for (const auto& hints : hints_vector) {
            test_host(hostname, hints);
        }
    }
}

auto main(int argc, char* argv[]) -> int
{
    try {
        const auto args {TestHost::parse_arguments(argc, argv)};
        const auto& context {Network::Context::instance()};

        if (TestHost::verbose) {
            std::cerr << context;
        }

        TestHost::test_host_invalid();

        if (args.size() > 1) {
            for (const auto& hostname : Network::skip_first(args)) {
                TestHost::test_host_valid(hostname);
            }
        }
        else {
            TestHost::test_host_valid(TestHost::get_hostname());
        }
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
