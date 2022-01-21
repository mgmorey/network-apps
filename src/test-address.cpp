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
#include "network/network.h"            // Address, Bytes, Context,
                                        // ErrorResult, Family,
                                        // Hostname, HostVector,
                                        // Overloaded, get_hosts(),
                                        // get_sa_family(),
                                        // get_sa_length(),
                                        // is_valid(), sin_size,
                                        // sin6_size

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

#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iomanip>      // std::right, std::setw()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <span>         // std::span
#include <string>       // std::string
#include <variant>      // std::visit()
#include <vector>       // std::vector

namespace TestAddress
{
    constexpr auto addr_data {Network::Byte {0xFFU}};
    constexpr auto addr_size {8};
    constexpr auto key_width {20};
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

    static auto print(const Network::Address& address) -> void
    {
        const auto length {address.length()};
        const auto family {address.family()};
        const auto data {address.data()};
        const auto port {address.port()};
        const auto size {address.size()};
        const auto text {address.text()};
        std::cout << "    "
                  << Network::Family(family)
                  << std::endl;
        std::cout << std::setw(key_width) << "        Size: "
                  << std::right << std::setw(value_width) << size
                  << std::endl;
        std::cout << std::setw(key_width) << "        Length: "
                  << std::right << std::setw(value_width) << length
                  << std::endl;
        std::cout << std::setw(key_width) << "        Port: "
                  << std::right << std::setw(value_width) << port
                  << std::endl;
        std::cout << std::setw(key_width) << "        Address: "
                  << std::right << std::setw(value_width) << text
                  << std::endl;
    }

    static auto print(const Network::Error& error) -> void
    {
        if (verbose) {
            std::cout << "Exception: "
                      << error.what()
                      << std::endl;
        }
    }

    static auto test_address() -> void
    {
        const Network::Bytes addr;
        const auto size {addr.size()};
        const auto length {Network::get_sa_length(addr)};
        const auto family {Network::get_sa_family(addr)};
        assert(size == 0);
        assert(length == 0);
        assert(family == AF_UNSPEC);
        assert(Network::is_valid(addr) == false);
    }

    static auto test_address_invalid() -> void
    {
        const Network::Bytes addr {addr_size, addr_data};
        const auto size {addr.size()};
        const auto length {Network::get_sa_length(addr)};
        const auto family {Network::get_sa_family(addr)};
        std::string what;
        assert(size == addr_size);
        assert(length == 0 || length == 0xFFU);
        assert(family == 0xFFU);
        assert(Network::is_valid(addr) == false);

        try {
            const Network::Address address {addr};
            static_cast<void>(address);
        }
        catch (const Network::LogicError& error) {
            print(error);
            what = error.what();
        }

        assert(what == "Invalid socket address: 0xFFFFFFFFFFFFFFFF");
    }

    static auto test_address_localhost(const Network::Bytes& addr) -> void
    {
        const Network::Address address {addr};
        const auto family {address.family()};
        const auto size {address.size()};
        const auto text {address.text()};

        switch (family) {
        case AF_INET:
            assert(size == Network::sin_size);
            break;
        case AF_INET6:
            assert(size == Network::sin6_size);
            break;
        default:
            assert(false);
        }

        assert(Network::is_valid(addr, verbose));
        print(address);

        switch (family) {
        case AF_INET:
            assert(text == "127.0.0.1");
            break;
        case AF_INET6:
            assert(text == "::1");
            break;
        default:
            assert(false);
        }
    }

    static auto test_address_localhost() -> void
    {
        static const Network::Hostname localhost {"localhost"};

        const auto hosts_result {Network::get_hosts(localhost)};
        std::visit(Network::Overloaded {
                [&](const Network::HostVector& hosts) {
                    std::cout << "Socket addresses for "
                              << localhost
                              << ": "
                              << std::endl;

                    for (const auto& host : hosts) {
                        test_address_localhost(host.address());
                    }
                },
                [&](const Network::ErrorResult& result) {
                    std::cout << "No "
                              << localhost
                              << " addresses: "
                              << result.string()
                              << std::endl;
                }
            }, hosts_result);
    }
}

auto main(int argc, char* argv[]) -> int
{
    try {
        const auto args {TestAddress::parse_arguments(argc, argv)};
        const auto& context {Network::Context::instance()};

        if (TestAddress::verbose) {
            std::cerr << context;
        }

        TestAddress::test_address();
        TestAddress::test_address_invalid();
        TestAddress::test_address_localhost();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
