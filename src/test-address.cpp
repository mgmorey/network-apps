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

#include "network/network.h"        // Address, AddrInfo, Hints,
                                    // Hostname, Overload, Service,
                                    // get_endpoint(), get_hosts()

#ifdef _WIN32
#include <getopt.h>     // getopt(), optarg, opterr, optind, optopt
#include <winsock2.h>   // AF_INET, AF_INET6, AF_UNSPEC, IPPROTO_TCP,
                        // SOCK_STREAM
#include <ws2tcpip.h>   // AI_ADDRCONFIG, AI_CANONNAME,
#else
#include <netdb.h>      // AI_ADDRCONFIG, AI_CANONNAME,
#include <netinet/in.h> // IPPROTO_TCP
#include <sys/socket.h> // AF_INET, AF_INET6, AF_UNSPEC, SOCK_STREAM
#include <unistd.h>     // getopt(), optarg, opterr, optind, optopt
#endif

#include <algorithm>    // std::for_each(), std::remove()
                        // std::unique()
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <optional>     // std::nullopt
#include <ostream>      // std::ostream
#include <span>         // std::span
#include <string>       // std::string
#include <utility>      // std::pair
#include <variant>      // std::variant, std::visit()
#include <vector>       // std::vector

namespace TestAddress
{
    static bool verbose {false};  // NOLINT

    template<typename T, typename U>
    auto erase(T& c, const U& value) -> void
    {
        c.erase(std::remove(c.begin(),
                            c.end(),
                            value),
                c.end());
    }

    template<typename T>
    auto unique(T& c) -> void
    {
        c.erase(std::unique(c.begin(),
                            c.end()),
                c.end());
    }

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
            const Network::Address address {t_host.address()};
            const auto endpoint_result {get_endpoint(address)};
            std::visit(Network::Overload {
                    [&](const Network::Endpoint& endpoint) {
                        Values values = {
                            address.text(),
                            endpoint.first.value_or(""),
                            t_host.canonical_name().value_or(""),
                        };
                        erase(values, "");
                        unique(values);
                        print(values);
                    },
                    [&](const Network::Result& result) {
                        std::cerr << result
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

    static auto get_description(const Network::Hints& hints) -> std::string
    {
        switch (hints.family()) {
        case AF_INET:
            return "IPv4";
        case AF_INET6:
            return "IPv6";
        default:
            return "";
        }
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

    static auto test_host(const Network::Hostname& host,
                          const Network::Hints& hints) -> void
    {
        const auto description {get_description(hints)};
        const auto hosts_result {Network::get_hosts(host, &hints)};
        std::visit(Network::Overload {
                [&](const Network::HostVector& hosts) {
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
                [&](const Network::Result& result) {
                    if (description.empty()) {
                        std::cout << "No";
                    }
                    else {
                        std::cout << "No "
                                  << description;
                    }

                    std::cout << " hosts:"
                              << std::endl
                              << result
                              << std::endl;
                }
            }, hosts_result);
    }

    static auto test_host(const Network::Hostname& host) -> void
    {
        if (host.has_value()) {
            const auto flags {AI_CANONNAME};
            std::cout << "Host: " << host << std::endl;
            Network::Hints hints4(AF_INET, SOCK_STREAM, IPPROTO_TCP, flags);
            Network::Hints hints6(AF_INET6, SOCK_STREAM, IPPROTO_TCP, flags);
            test_host(host, hints4);
            test_host(host, hints6);
        }
        else {
            const auto flags {AI_ADDRCONFIG | AI_CANONNAME};
            Network::Hints hints(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP, flags);
            test_host(host, hints);
        }
    }
}

auto main(int argc, char* argv[]) -> int
{
    try {
        const auto args {TestAddress::parse_arguments(argc, argv)};
        const Network::Context context(TestAddress::verbose);

        if (context.result()) {
            std::cerr << context.result()
                      << std::endl;
        }
        else if (args.size() > 1) {
            TestAddress::test_host(args[1]);
        }
        else {
            TestAddress::test_host(std::nullopt);
        }

        static_cast<void>(context);
    }
    catch (std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
