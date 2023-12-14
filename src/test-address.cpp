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
#include "network/network.h"            // Address, Context,
                                        // HostVector, Hostname,
                                        // OsErrorResult, Overloaded,
                                        // SocketFamily, get_hosts(),
                                        // get_sa_family(),
                                        // get_sa_length(), sin_size,
                                        // sin6_size
#include "network/parse.h"              // parse()

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_UNSPEC,
                            // IPPROTO_TCP, SOCK_STREAM
#include <ws2tcpip.h>       // AI_ADDRCONFIG, AI_CANONNAME
#else
#include <netdb.h>          // AI_ADDRCONFIG, AI_CANONNAME
#include <netinet/in.h>     // IPPROTO_TCP
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_UNSPEC,
                            // SOCK_STREAM
#endif

#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iomanip>      // std::right, std::setw()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <variant>      // std::visit()

namespace TestAddress
{
    using Network::Address;
    using Network::Byte;
    using Network::ByteString;
    using Network::Context;
    using Network::Error;
    using Network::HostVector;
    using Network::Hostname;
    using Network::OsErrorResult;
    using Network::Overloaded;
    using Network::SocketHints;
    using Network::SocketFamily;
    using Network::get_hosts;
    using Network::get_sa_family;
    using Network::get_sa_length;
    using Network::parse;

    static constexpr auto print_key_width {20};
    static constexpr auto print_value_width {10};

    static constexpr SocketHints unspec {
        AI_CANONNAME, AF_UNSPEC, SOCK_STREAM
    };

    static bool verbose {false};  // NOLINT

    auto parse_arguments(int argc, char** argv) -> void
    {
        const auto [_, options] {parse(argc, argv, "v")};

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

        static_cast<void>(_);
    }

    auto print(const Address& address) -> void
    {
        const auto length {address.length()};
        const auto family {address.family()};
        const auto port {address.port()};
        const auto size {address.size()};
        const auto text {address.text()};
        std::cout << "    "
                  << SocketFamily(family)
                  << std::endl
                  << std::setw(print_key_width) << "        Size: "
                  << std::right << std::setw(print_value_width) << size
                  << std::endl
                  << std::setw(print_key_width) << "        Length: "
                  << std::right << std::setw(print_value_width) << length
                  << std::endl
                  << std::setw(print_key_width) << "        Port: "
                  << std::right << std::setw(print_value_width) << port
                  << std::endl
                  << std::setw(print_key_width) << "        Address: "
                  << std::right << std::setw(print_value_width) << text
                  << std::endl;
    }

    auto print(const Error& error) -> void
    {
        if (verbose) {
            std::cout << "Exception: "
                      << error.what()
                      << std::endl;
        }
    }

    auto test_address_empty() -> void
    {
        const ByteString addr;
        assert(addr.empty());
        assert(get_sa_family(addr) == AF_UNSPEC);
        assert(get_sa_length(addr) == 0U);
    }

    auto test_address_valid(const ByteString& addr) -> void
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

    auto test_address_valid() -> void
    {
        static const Hostname localhost {"localhost"};

        const auto hosts_result {get_hosts(localhost, unspec)};
        std::visit(Overloaded {
                [&](const HostVector& hosts) {
                    std::cout << "Socket addresses for "
                              << localhost
                              << ": "
                              << std::endl;

                    for (const auto& host : hosts) {
                        test_address_valid(host.address());
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
        const auto& context {Context::instance()};
        parse_arguments(argc, argv);

        if (verbose) {
            std::cout << context << std::endl;
        }

        test_address_valid();
        test_address_empty();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
