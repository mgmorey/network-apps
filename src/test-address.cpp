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

#include "network/assert.hpp"           // assert()
#include "network/network.hpp"          // Address, ByteSpan, Error,
                                        // Hostname, SharedRuntime,
                                        // SocketFamily, SocketHints,
                                        // SocketHost, SocketLimits,
                                        // insert(), run(),
                                        // sa_family_type, sa_size,
                                        // sin_family_type,
                                        // sun_length_max,
                                        // sun_length_min,
                                        // sun_path_size, sun_size,
                                        // to_bytestring(), validate()
#include "network/os-features.hpp"      // HAVE_SOCKADDR_SA_LEN
#include "network/parse.hpp"            // parse()

#ifdef _WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                            // IPPROTO_TCP, SOCK_STREAM, sockaddr,
                            // sockaddr_in, sockaddr_in6,
                            // sockaddr_storage
#include <ws2tcpip.h>       // AI_ADDRCONFIG, AI_CANONNAME
#else
#include <netdb.h>          // AI_ADDRCONFIG, AI_CANONNAME
#include <netinet/in.h>     // IPPROTO_TCP
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                            // sockaddr, sockaddr_in6, sockaddr_in,
                            // sockaddr_un
#endif

#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <cstring>      // std::memset()
#include <iomanip>      // std::right, std::setw()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <iterator>     // std::back_inserter()
#include <regex>        // std::regex, std::regex_match
#include <string>       // std::string
#include <vector>       // std::vector

namespace
{
    using Network::Address;
    using Network::ByteSpan;
    using Network::ByteString;
    using Network::Error;
    using Network::Hostname;
    using Network::SharedRuntime;
    using Network::SocketFamily;
    using Network::SocketHints;
    using Network::SocketHost;
    using Network::SocketLimits;
#ifndef _WIN32
    using Network::af_unix;
#endif
    using Network::af_unspec;
    using Network::insert;
    using Network::parse;
#ifndef _WIN32
    using Network::path_length_max;
#endif
    using Network::run;
    using Network::sa_family_type;
    using Network::sa_size;
    using Network::sin_family_type;
    using Network::string_null;
#ifndef _WIN32
    using Network::sun_length_max;
    using Network::sun_length_min;
    using Network::sun_path_size;
    using Network::sun_size;
#endif
    using Network::to_bytestring;
    using Network::validate;

    constexpr auto expected_family_re {
        R"(Invalid socket address family: (\d+))"
    };
    constexpr auto expected_length_re {
        R"(Value (\d+) is out of range \[\d+, \d+\] of \w+_type)"
    };
    constexpr auto print_key_width {20};
    constexpr auto print_value_width {10};

    const SocketHints unspec {af_unspec, SOCK_STREAM, 0, AI_CANONNAME};

    auto is_verbose {false};	// NOLINT
    Address previous_address;		// NOLINT

    auto create_sa(sa_family_type family,
                   [[maybe_unused]] std::size_t length = sa_size) -> sockaddr
    {
        sockaddr sa {};
#ifdef HAVE_SOCKADDR_SA_LEN
        sa.sa_len = length;
#endif
        sa.sa_family = family;
        return sa;
    }

    auto create_sin(sin_family_type family = AF_INET) -> sockaddr_in
    {
        sockaddr_in sin {};
#ifdef HAVE_SOCKADDR_SA_LEN
        sin.sin_len = sizeof sin;
#endif
        sin.sin_family = family;
        return sin;
    }

    auto create_sin6(sin_family_type family = AF_INET6) -> sockaddr_in6
    {
        sockaddr_in6 sin6 {};
#ifdef HAVE_SOCKADDR_SA_LEN
        sin6.sin6_len = sizeof sin6;
#endif
        sin6.sin6_family = family;
        return sin6;
    }

#ifndef _WIN32

    auto create_sun(sa_family_type family = af_unix,
                    [[maybe_unused]] std::size_t sun_len = sun_size,
                    std::size_t path_len = 0UL) -> sockaddr_un
    {
        sockaddr_un sun {};
#ifdef HAVE_SOCKADDR_SA_LEN
        sun.sun_len = sun_len;
#endif
        sun.sun_family = family;

        if (path_len != 0UL) {
            std::memset(&sun.sun_path, 'X', path_len);
        }

        return sun;
    }

#endif

    auto parse_arguments(int argc, char** argv) -> void
    {
        [[maybe_unused]] const auto [args, options] {parse(argc, argv, "v")};

        if (options.contains('?')) {
            std::cerr << "Usage: "
                      << *argv
                      << " [-v]"
                      << std::endl;
            std::exit(EXIT_FAILURE);
        }

        if (options.contains('v')) {
            is_verbose = true;
        }
    }

    auto print(ByteSpan bs) -> void
    {
        const Address address {bs};
        const auto family {address.family()};
#ifdef HAVE_SOCKADDR_SA_LEN
        const auto length {address.length()};
#endif
        const auto port {address.port()};
        const auto text {address.text()};
        std::cout << "    "
                  << SocketFamily(family)
                  << std::endl
                  << std::setw(print_key_width) << "        Size: "
                  << std::right << std::setw(print_value_width) << bs.size()
                  << std::endl
#ifdef HAVE_SOCKADDR_SA_LEN
                  << std::setw(print_key_width) << "        Length: "
                  << std::right << std::setw(print_value_width) << length
                  << std::endl
#endif
                  << std::setw(print_key_width) << "        Port: "
                  << std::right << std::setw(print_value_width) << port
                  << std::endl
                  << std::setw(print_key_width) << "        Address: "
                  << std::right << std::setw(print_value_width) << text
                  << std::endl;
    }

    auto print(const Error& error) -> void
    {
        if (is_verbose) {
            std::cout << "Exception: "
                      << error.what()
                      << std::endl;
        }
    }

    auto test(ByteSpan bs) -> void
    {
        assert(!bs.empty());
        const auto size {bs.size()};
        const Address address {bs};
        assert(!address.empty());
        const auto family {address.family()};
        const auto [size_min, size_max] {SocketLimits(family).limits()};
        assert(size_min <= size);
        assert(size <= size_max);
#ifdef HAVE_SOCKADDR_SA_LEN
        static_cast<void>(address.length());
#endif
        if (family != AF_UNIX) {
            static_cast<void>(address.port());
        }

        const auto text {address.text()};
        static_cast<void>(text);
        ByteString addr {bs.begin(), bs.end()};
        assert(ByteString {address} == addr);
        previous_address = addr;
    }

    auto test_empty() -> void
    {
        const Address address {ByteString {}};
        assert(address.empty());
        assert(address.port() == 0);
        assert(address.text() == string_null);
    }

    auto test_sa(const sockaddr& sa, std::size_t sa_len,
                 const std::string& expected_re) -> void
    {
        const std::regex expected_regex {expected_re};
        std::string actual_str;

        try {
            validate(&sa, sa_len);
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        if (!expected_re.empty()) {
            assert(std::regex_match(actual_str, expected_regex));
        }

        actual_str.clear();

        try {
            test(validate(to_bytestring(&sa, sa_len)));
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        if (!expected_re.empty()) {
            assert(std::regex_match(actual_str, expected_regex));
        }
    }

    auto test_sa_invalid_length() -> void
    {
        const auto sa {create_sa(AF_UNSPEC, 0)};
        test_sa(sa, 0, expected_length_re);
    }

    auto test_sin(const sockaddr_in& sin,
                  const std::string& expected_re) -> void
    {
        const std::regex expected_regex {expected_re};
        std::string actual_str;

        try {
            validate(&sin);
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        assert(std::regex_match(actual_str, expected_regex));

        if (sin.sin_family == AF_INET) {
            actual_str.clear();

            try {
                test(validate(to_bytestring(&sin, sizeof sin)));
            }
            catch (const Error& error) {
                print(error);
                actual_str = error.what();
            }

            assert(std::regex_match(actual_str, expected_regex));
        }
    }

    auto test_sin_invalid_family() -> void
    {
        const auto sin {create_sin(AF_UNSPEC)};
        test_sin(sin, expected_family_re);
    }

    auto test_sin_valid() -> void
    {
        const auto sin {create_sin()};
        test_sin(sin, {});
    }

    auto test_sin6(const sockaddr_in6& sin6,
                   const std::string& expected_re) -> void
    {
        const std::regex expected_regex {expected_re};
        std::string actual_str;

        try {
            validate(&sin6);
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        assert(std::regex_match(actual_str, expected_regex));

        if (sin6.sin6_family == AF_INET6) {
            actual_str.clear();

            try {
                test(validate(to_bytestring(&sin6, sizeof sin6)));
            }
            catch (const Error& error) {
                print(error);
                actual_str = error.what();
            }

            assert(std::regex_match(actual_str, expected_regex));
        }
    }

    auto test_sin6_invalid_family() -> void
    {
        const auto sin6 {create_sin6(AF_UNSPEC)};
        test_sin6(sin6, expected_family_re);
    }

    auto test_sin6_valid() -> void
    {
        const auto sin6 {create_sin6()};
        test_sin6(sin6, {});
    }

#ifndef _WIN32

    auto test_path(const std::string& path,
                   const std::string& expected_re) -> void
    {
        const std::regex expected_regex {expected_re};
        std::string actual_str;

        try {
            validate(path);
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        assert(std::regex_match(actual_str, expected_regex));
    }

    auto test_path_invalid_length() -> void
    {
        const std::string path(path_length_max + 1, '.' );
        test_path(path, expected_length_re);
    }

    auto test_sun(const sockaddr_un& sun, std::size_t sun_len,
                  const std::string& expected_re) -> void
    {
        const std::regex expected_regex {expected_re};
        std::string actual_str;

        try {
            validate(&sun, sun_len);
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        assert(std::regex_match(actual_str, expected_regex));

        if (sun.sun_family == af_unix) {
            actual_str.clear();

            try {
                test(validate(to_bytestring(&sun, sun_len)));
            }
            catch (const Error& error) {
                print(error);
                actual_str = error.what();
            }

            assert(std::regex_match(actual_str, expected_regex));
        }
    }

    auto test_sun_invalid_family() -> void
    {
        const auto sun {create_sun(AF_UNSPEC, sun_length_min)};
        test_sun(sun, sun_length_min, expected_family_re);
    }

    auto test_sun_valid_path_large() -> void
    {
        const auto sun {create_sun(af_unix, sun_length_max, sun_path_size)};
        test_sun(sun, sun_length_max, {});
    }

    auto test_sun_valid_path_small() -> void
    {
        const auto sun {create_sun(af_unix, sun_length_min)};
        test_sun(sun, sun_length_min, {});
    }

#endif

    auto test(const SharedRuntime& sr) -> void
    {
        static const Hostname localhost {"localhost"};

        std::vector<SocketHost> hosts;
        auto it {std::back_inserter(hosts)};
        const auto result {insert(it, localhost, {}, unspec, sr)};

        if (result) {
            std::cout << "No " << localhost << " addresses: " << result.string()
                      << std::endl;
        }
        else {
            std::cout << "Socket addresses for "
                      << localhost
                      << ": "
                      << std::endl;

            for (const auto& host : hosts) {
                const ByteSpan bs {host.address()};
                print(bs);
                test(bs);
            }
        }
    }
}

auto main(int argc, char* argv[]) -> int
{
    try {
        parse_arguments(argc, argv);
        const auto rt {run(is_verbose)};

        if (is_verbose) {
            std::cout << *rt << std::endl;
        }

        test_empty();
        test_sa_invalid_length();
        test_sin6_invalid_family();
        test_sin_invalid_family();
#ifndef _WIN32
        test_path_invalid_length();
        test_sun_invalid_family();
        test_sun_valid_path_large();
        test_sun_valid_path_small();
#endif
        test_sin_valid();
        test_sin6_valid();
        test(rt);
    }
    catch (const Error& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
