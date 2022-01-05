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
                            // IPPROTO_TCP, SOCK_STREAM, sockaddr_in
#include <ws2tcpip.h>       // AI_ADDRCONFIG, AI_CANONNAME
#else
#include <netdb.h>          // AI_ADDRCONFIG, AI_CANONNAME
#include <netinet/in.h>     // IPPROTO_TCP, sockaddr_in
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_UNSPEC,
                            // SOCK_STREAM
#include <unistd.h>         // getopt(), optarg, opterr, optind
#endif

#include <cassert>      // assert()
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iomanip>      // std::left, std::right, std::setw()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <span>         // std::span
#include <string>       // std::string
#include <variant>      // std::visit()
#include <vector>       // std::vector

namespace TestAddress
{
    constexpr auto key_width {15};
    constexpr auto value_width {10};

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

    static auto test_address_localhost(const Network::Host& host) -> void
    {
        const auto addr {host.address()};
        const auto size {addr.size()};
        std::cout << std::left
                  << std::setw(key_width)
                  << "    Size: "
                  << std::left
                  << std::setw(value_width)
                  << size
                  << std::endl;
        const Network::Address address {addr};
        const auto length {address.length()};
        std::cout << std::left
                  << std::setw(key_width)
                  << "    Length: "
                  << std::left
                  << std::setw(value_width)
                  << length
                  << std::endl;
        const Network::Family family {address.family()};
        std::cout << std::left
                  << std::setw(key_width)
                  << "    Family: "
                  << std::left
                  << std::setw(value_width)
                  << family
                  << std::endl;
        const auto text {address.text()};
        std::cout << std::left
                  << std::setw(key_width)
                  << "    Address: "
                  << std::left
                  << std::setw(value_width)
                  << text
                  << std::endl;
        assert(size == sizeof(sockaddr_in));	// NOLINT
        assert(length <= addr.size());		// NOLINT
        assert(family == AF_INET);		// NOLINT
        assert(text == "127.0.0.1");		// NOLINT
    }

    static auto test_address_localhost() -> void
    {
        const Network::Hostname localhost {"localhost"};
        const Network::Hints hints
            {AF_INET, SOCK_STREAM, IPPROTO_TCP, AI_ADDRCONFIG};
        const auto hosts_result {Network::get_hosts(localhost, &hints)};
        std::visit(Network::Overload {
                [&](const Network::HostVector& hosts) {
                    std::cout << "Socket address for local host: "
                              << std::endl;
                    assert(hosts.size() == 1);  // NOLINT
                    test_address_localhost(hosts[0]);
                },
                [&](const Network::Result& result) {
                    std::cout << "No local host address: "
                              << result
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
