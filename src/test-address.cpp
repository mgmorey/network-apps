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
                                        // Family, Hostname,
                                        // HostVector, OsErrorResult,
                                        // Overloaded, get_hosts(),
                                        // get_sa_family(),
                                        // get_sa_length(),
                                        // is_valid(), sin_size,
                                        // sin6_size

#ifdef WIN32
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

#include <cstdlib>      // EXIT_FAILURE, std::exit(), std::size_t
#include <exception>    // std::exception
#include <iomanip>      // std::right, std::setw()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <span>         // std::span
#include <string>       // std::string
#include <variant>      // std::visit()
#include <vector>       // std::vector

using Network::Address;
using Network::Byte;
using Network::ByteString;
using Network::Context;
using Network::Error;
using Network::HostVector;
using Network::Hostname;
using Network::LogicError;
using Network::OsErrorResult;
using Network::Overloaded;
using Network::SocketFamily;
using Network::get_hosts;
using Network::get_sa_family;
using Network::get_sa_length;
using Network::is_valid;

namespace TestAddress
{
    constexpr auto expected_error_invalid_address {
        "Invalid socket address: 0xFFFFFFFFFFFFFFFF"
    };
    constexpr auto invalid_addr_data {Byte {0xFFU}};
    constexpr auto invalid_addr_size {8};
    constexpr auto print_key_width {20};
    constexpr auto print_value_width {10};

    static bool verbose {false};  // NOLINT

    static auto parse_arguments(int argc, char** argv) ->
        std::vector<std::string>
    {
        std::vector<std::string> result {*argv};
        int opt {};

        while ((opt = ::getopt(argc, argv, "v")) != -1) {
            switch (opt) {
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
            const auto i {static_cast<std::size_t>(index)};
            result.emplace_back(args[i]);
        }

        return result;
    }

    static auto print(const Address& address) -> void
    {
        const auto length {address.length()};
        const auto family {address.family()};
        const auto port {address.port()};
        const auto size {address.size()};
        const auto text {address.text()};
        std::cout << "    "
                  << SocketFamily(family)
                  << std::endl;
        std::cout << std::setw(print_key_width) << "        Size: "
                  << std::right << std::setw(print_value_width) << size
                  << std::endl;
        std::cout << std::setw(print_key_width) << "        Length: "
                  << std::right << std::setw(print_value_width) << length
                  << std::endl;
        std::cout << std::setw(print_key_width) << "        Port: "
                  << std::right << std::setw(print_value_width) << port
                  << std::endl;
        std::cout << std::setw(print_key_width) << "        Address: "
                  << std::right << std::setw(print_value_width) << text
                  << std::endl;
    }

    static auto print(const Error& error) -> void
    {
        if (verbose) {
            std::cout << "Exception: "
                      << error.what()
                      << std::endl;
        }
    }

    static auto test_address() -> void
    {
        const ByteString addr;
        const auto size {addr.size()};
        const auto length {get_sa_length(addr)};
        const auto family {get_sa_family(addr)};
        assert(size == 0);
        assert(length == 0);
        assert(family == AF_UNSPEC);
        assert(is_valid(addr) == false);
    }

    static auto test_address_invalid() -> void
    {
        std::string actual_error_str;

        try {
            const ByteString addr {invalid_addr_size, invalid_addr_data};
            const Address address {addr};
            static_cast<void>(address);
        }
        catch (const LogicError& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str == expected_error_invalid_address);
    }

    static auto test_address_localhost(const ByteString& addr) -> void
    {
        const Address address {addr};
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

        assert(is_valid(addr, verbose));
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
        static const Hostname localhost {"localhost"};

        const auto hosts_result {get_hosts(localhost)};
        std::visit(Overloaded {
                [&](const HostVector& hosts) {
                    std::cout << "Socket addresses for "
                              << localhost
                              << ": "
                              << std::endl;

                    for (const auto& host : hosts) {
                        test_address_localhost(host.address());
                    }
                },
                [&](const OsErrorResult& result) {
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
    using namespace TestAddress;

    try {
        const auto args {parse_arguments(argc, argv)};
        const auto& context {Context::instance()};

        if (verbose) {
            std::cout << context;
        }

        test_address();
        test_address_invalid();
        test_address_localhost();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
