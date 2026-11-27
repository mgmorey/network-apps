// Copyright (C) 2026  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/gtest.hpp"
#include "network/argumentdata.hpp"             // ArgumentData
#include "network/create-socket-hints.hpp"      // create_socket()
#include "network/error.hpp"                    // Error
#include "network/get-endpointresult.hpp"       // get_endpointresult()
#include "network/ipsockethints.hpp"            // IpSocketHints
#include "network/parse.hpp"                    // parse()
#include "network/run.hpp"                      // run()
#include "network/runtimescope.hpp"             // RuntimeScope
#include "network/to-string-void.hpp"           // to_string()

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, SOCK_STREAM, sockaddr_in
#include <ws2tcpip.h>   // NI_NUMERICHOST, NI_NUMERICSERV
#else
#include <netdb.h>      // NI_NUMERICHOST, NI_NUMERICSERV
#include <netinet/in.h> // sockaddr_in
#endif

#ifndef _WIN32
#include "network/get-path-length.hpp"          // get_path_length()
#include "network/get-sun-length.hpp"           // get_sun_length()
#include "network/os-features.hpp"              // HAVE_SOCKADDR_SA_LEN
#include "network/sun-offsets.hpp"              // sun_path_offset

#include <sys/un.h>     // sockaddr_un

#include <cstring>      // std::memcpy()
#endif

#include <array>        // std::array
#include <span>         // std::as_bytes(), std::span
#include <string>       // std::string
#include <vector>       // std::vector

namespace
{
    using Network::ArgumentData;
    using Network::Error;
    using Network::IpSocketHints;
    using Network::af_ip_v4;
    using Network::create_socket;
    using Network::get_endpointresult;
    using Network::ipproto_tcp;
    using Network::parse;
    using Network::run;
    using Network::RuntimeScope;
    using Network::to_string;

    // GoogleTest assertions inflate the cognitive-complexity score.
    // NOLINTNEXTLINE(readability-function-cognitive-complexity)
    auto test_ip_socket_hints() -> void
    {
        const IpSocketHints full {af_ip_v4, SOCK_STREAM, ipproto_tcp,
                                  AI_PASSIVE,};
        EXPECT_EQ(full.m_family, AF_INET);
        EXPECT_EQ(full.m_socktype, SOCK_STREAM);
        EXPECT_EQ(full.m_protocol, IPPROTO_TCP);
        EXPECT_EQ(full.m_flags, AI_PASSIVE);

        const IpSocketHints family_and_type {af_ip_v4, SOCK_STREAM,
                                             AI_PASSIVE,};
        EXPECT_EQ(family_and_type.m_family, AF_INET);
        EXPECT_EQ(family_and_type.m_socktype, SOCK_STREAM);
        EXPECT_EQ(family_and_type.m_protocol, 0);
        EXPECT_EQ(family_and_type.m_flags, AI_PASSIVE);

        const IpSocketHints family_and_protocol {af_ip_v4, ipproto_tcp,
                                                 AI_PASSIVE,};
        EXPECT_EQ(family_and_protocol.m_family, AF_INET);
        EXPECT_EQ(family_and_protocol.m_socktype, 0);
        EXPECT_EQ(family_and_protocol.m_protocol, IPPROTO_TCP);
        EXPECT_EQ(family_and_protocol.m_flags, AI_PASSIVE);

        const IpSocketHints type_only {SOCK_STREAM, AI_PASSIVE};
        EXPECT_EQ(type_only.m_family, 0);
        EXPECT_EQ(type_only.m_socktype, SOCK_STREAM);
        EXPECT_EQ(type_only.m_protocol, 0);
        EXPECT_EQ(type_only.m_flags, AI_PASSIVE);

        const IpSocketHints protocol_only {ipproto_tcp, AI_PASSIVE};
        EXPECT_EQ(protocol_only.m_family, 0);
        EXPECT_EQ(protocol_only.m_socktype, 0);
        EXPECT_EQ(protocol_only.m_protocol, IPPROTO_TCP);
        EXPECT_EQ(protocol_only.m_flags, AI_PASSIVE);
    }

    auto test_parse_argv() -> void
    {
        const std::vector<std::string> values {
            "test-misc", "-v", "payload",
        };
        ArgumentData data {values};
        const auto [operands, options] {
            parse(static_cast<int>(data.size()), data.data(), "v"),
        };

        ASSERT_EQ(operands.size(), 1U);
        EXPECT_STREQ(operands.front(), "payload");
        EXPECT_TRUE(options.contains('v'));
    }

    auto test_to_string_void() -> void
    {
        constexpr std::array bytes {'a', 'b', 'c', '\0'};
        EXPECT_EQ(to_string(bytes.data(), bytes.size() - 1), "0x616263");
    }

    // GoogleTest assertions inflate the cognitive-complexity score.
    // NOLINTNEXTLINE(readability-function-cognitive-complexity)
    auto test_verbose_socket_creation() -> void
    {
        const auto sr {run(RuntimeScope::shared, true)};
        const IpSocketHints valid {af_ip_v4, SOCK_STREAM};
        const auto socket {create_socket(valid, sr.get())};
        ASSERT_TRUE(socket);

        const Network::SocketHints invalid {-1, SOCK_STREAM, 0};
        EXPECT_THROW(static_cast<void>(create_socket(invalid, sr.get())),
                     Error);
    }

    auto test_verbose_endpointresult() -> void
    {
        const auto sr {run(RuntimeScope::shared, true)};
        sockaddr_in address {};
        address.sin_family = AF_INET;
#ifdef HAVE_SOCKADDR_SA_LEN
        address.sin_len =
            static_cast<decltype(address.sin_len)>(sizeof(address));
#endif
        const auto bytes {std::as_bytes(std::span {&address, 1})};
        std::array<char, NI_MAXHOST> hostname {};
        std::array<char, NI_MAXSERV> service {};
        const auto success {get_endpointresult(hostname,
                                               service,
                                               bytes,
                                               NI_NUMERICHOST |
                                                   NI_NUMERICSERV,
                                               sr.get()),};
        EXPECT_FALSE(success) << success.string();

        std::array<char, 1> short_hostname {};
        const auto error {get_endpointresult(short_hostname,
                                             service,
                                             bytes,
                                             NI_NUMERICHOST |
                                                 NI_NUMERICSERV,
                                             sr.get()),};
        EXPECT_TRUE(error);
    }

#ifndef _WIN32
    auto test_unix_socket_lengths() -> void
    {
        constexpr std::array path {'a', 'b', 'c', '\0'};
        sockaddr_un sun {};
        std::memcpy(static_cast<void*>(sun.sun_path),
                    path.data(),
                    path.size());

        EXPECT_EQ(Network::get_path_length(&sun, Network::sun_path_offset), 0U);
        EXPECT_EQ(Network::get_sun_length(&sun, Network::sun_path_offset),
                  Network::sun_path_offset);

        const auto sun_length {Network::sun_path_offset + sizeof(path)};
        EXPECT_EQ(Network::get_path_length(&sun, sun_length),
                  sizeof(path) - 1);
        EXPECT_EQ(Network::get_sun_length(&sun, sun_length), sun_length);
    }
#endif
}

TEST(Miscellaneous, IpSocketHintsConstructors)
{
    test_ip_socket_hints();
}

TEST(Miscellaneous, ParseArgv)
{
    test_parse_argv();
}

TEST(Miscellaneous, ToStringVoid)
{
    test_to_string_void();
}

TEST(Miscellaneous, VerboseSocketCreation)
{
    test_verbose_socket_creation();
}

TEST(Miscellaneous, VerboseEndpointResult)
{
    test_verbose_endpointresult();
}

#ifndef _WIN32
TEST(Miscellaneous, UnixSocketLengths)
{
    test_unix_socket_lengths();
}
#endif
