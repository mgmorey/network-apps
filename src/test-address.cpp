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

#include "network/network.h"            // Address, Bytes, Hints,
                                        // Hostname, Overload,
                                        // get_hosts()

#ifdef _WIN32
#include <getopt.h>         // getopt(), optarg, opterr, optind
#include <winsock2.h>       // AF_INET, AF_INET6, AF_UNSPEC,
                            // IPPROTO_TCP, SOCK_STREAM
#include <ws2tcpip.h>       // AI_ADDRCONFIG, AI_CANONNAME
#else
#include <netdb.h>          // AI_ADDRCONFIG, AI_CANONNAME
#include <netinet/in.h>     // IPPROTO_TCP
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_UNSPEC,
                            // SOCK_STREAM
#include <unistd.h>         // getopt(), optarg, opterr, optind
#endif

#include <cassert>      // assert()
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <span>         // std::span
#include <string>       // std::string
#include <variant>      // std::visit()
#include <vector>       // std::vector

namespace TestAddress
{
    static bool verbose {false};  // NOLINT

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

    static auto test_address_empty() -> void
    {
        const Network::Bytes addr;
        assert(Network::get_sa_length(addr, 0) == 0);		// NOLINT
        assert(Network::get_sa_family(addr, 0) == AF_UNSPEC);	// NOLINT
        assert(Network::is_valid(addr) == false);		// NOLINT
    }

    static auto test_address_localhost() -> void
    {
        const Network::Hints hints {AF_INET, 0, 0, 0};
        const Network::Hostname localhost {"localhost"};
        std::cout << "Socket address for host: "
                  << localhost
                  << std::endl;
        const auto hosts_result {Network::get_hosts(localhost, &hints)};
        std::visit(Network::Overload {
                [&](const Network::HostVector& hosts) {
                    assert(hosts.size() == 1);			// NOLINT
                    const auto host {hosts[0]};
                    const auto addr {host.address()};
                    const Network::Address address {addr};
                    const auto length {address.length()};
                    std::cout << "    "
                              << "Length: "
                              << length
                              << std::endl;
                    const auto family {address.family()};
                    std::cout << "    "
                              << "Family: "
                              << family
                              << std::endl;
                    const auto port {address.port()};
                    std::cout << "    "
                              << "Port: "
                              << port
                              << std::endl;
                    const auto text {address.text()};
                    std::cout << "    "
                              << "Address: "
                              << text
                              << std::endl;
                    assert(family == AF_INET);			// NOLINT
                    assert(port == 0);				// NOLINT
                    assert(text == "127.0.0.1");		// NOLINT
                },
                [&](const Network::Result& result) {
                    std::cout << result
                              << std::endl;
                }
            }, hosts_result);
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
        else {
            TestAddress::test_address_empty();
            TestAddress::test_address_localhost();
        }

        static_cast<void>(context);
    }
    catch (std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
