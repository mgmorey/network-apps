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

#include "network/argumentspan.h"       // ArgumentSpan
#include "network/assert.h"             // assert()
#include "network/network.h"            // Address, ByteString,
                                        // Endpoint, HostnameView,
                                        // IpSocketHints,
                                        // OptionalHints,
                                        // OptionalHostname,
                                        // OsErrorResult, SocketHints,
                                        // SocketHost, af_ip_v4,
                                        // af_ip_v6, always_false_v,
                                        // get_hostname(), insert(),
                                        // os_error_type,
                                        // start_context, uniquify()
#include "network/parse.h"              // parse()

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_UNSPEC,
                            // IPPROTO_TCP, SOCK_STREAM,
                            // WSAHOST_NOT_FOUND
#include <ws2tcpip.h>       // AI_ADDRCONFIG, AI_CANONNAME
#else
#include <netdb.h>          // AI_ADDRCONFIG, AI_CANONNAME, EAI_AGAIN,
                            // EAI_FAMILY, EAI_NODATA, EAI_NONAME,
                            // EAI_SOCKTYPE
#include <netinet/in.h>     // IPPROTO_TCP
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_UNSPEC,
                            // SOCK_STREAM
#endif

#include <algorithm>    // std::ranges
#include <cstdlib>      // EXIT_FAILURE, std::exit(), std::size_t
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <iterator>     // std::back_inserter()
#include <list>         // std::list
#include <ostream>      // std::ostream
#include <set>          // std::set
#include <string>       // std::string
#include <vector>       // std::vector

namespace
{
    using Network::Address;
    using Network::ArgumentSpan;
    using Network::ByteString;
    using Network::Endpoint;
    using Network::HostnameView;
    using Network::IpSocketHints;
    using Network::OptionalHints;
    using Network::OptionalHostname;
    using Network::OsErrorResult;
    using Network::SocketHints;
    using Network::SocketHost;
    using Network::always_false_v;
    using Network::get_hostname;
    using Network::insert;
    using Network::os_error_type;
    using Network::parse;
    using Network::start_context;
    using Network::uniquify;

    using ErrorCodeSet = std::set<os_error_type>;
    using StringList = std::list<std::string>;

    constexpr auto localhost {"localhost"};

    const IpSocketHints ip_any {SOCK_STREAM, AI_CANONNAME};

    auto is_verbose {false};  // NOLINT

    class Print
    {
    public:
        static auto get_endpoint(const ByteString& addr) -> Endpoint
        {
            return Network::get_endpoint(addr, 0, is_verbose);
        }

        explicit Print(std::ostream& t_os) :
            m_os(t_os)
        {
        }

        auto operator()(const SocketHost& t_host) -> void
        {
            const auto& addr {t_host.address()};
            const auto endpoint {get_endpoint(addr)};
            StringList list = {
                Address(addr).text(),
                endpoint.at(0),
                t_host.canonical_name().value_or(std::string {})
            };
            list.remove(std::string {});
            uniquify(list);
            print(list);
        }

        auto print(const StringList& list) -> void
        {
            if (list.empty()) {
                return;
            }

            auto element {list.begin()};
            m_os << "    ";

            for (std::size_t i = 0; i < list.size(); ++i) {
                if (i > 1) {
                    m_os << ", ";
                }

                m_os << *element++;

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

    auto get_codes_family() -> const ErrorCodeSet&
    {
#if defined(WIN32)
        static const ErrorCodeSet codes = {WSAEAFNOSUPPORT};
#else
        static const ErrorCodeSet codes = {EAI_FAMILY};
#endif
        return codes;
    }

#ifndef OS_CYGWIN_NT
    auto get_codes_protocol() -> const ErrorCodeSet&
    {
#if defined(WIN32)
        static const ErrorCodeSet codes = {0};
#elif defined(OS_DARWIN)
        static const ErrorCodeSet codes = {EAI_BADHINTS};
#else
        static const ErrorCodeSet codes = {EAI_SOCKTYPE};
#endif
        return codes;
    }
#endif

    auto get_codes_socktype() -> const ErrorCodeSet&
    {
#if defined(WIN32)
        static const ErrorCodeSet codes = {WSAESOCKTNOSUPPORT};
#elif defined(OS_DARWIN)
        static const ErrorCodeSet codes = {EAI_BADHINTS};
#else
        static const ErrorCodeSet codes = {EAI_SOCKTYPE};
#endif
        return codes;
    }

    auto parse_arguments(int argc, char** argv) -> ArgumentSpan
    {
        const auto [operands, options] {parse(argc, argv, "v")};

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

        return operands;
    }

    auto print(const OsErrorResult& result) -> void
    {
        std::cout << result.string()
                  << std::endl;
    }

    auto print(const std::vector<SocketHost>& hosts) -> void
    {
        if (hosts.empty()) {
            return;
        }

        std::ranges::for_each(hosts, Print(std::cout));
    }

    auto test_host(const HostnameView& host,
                   const OptionalHints& hints,
                   const ErrorCodeSet& expected_codes = {0}) -> void
    {
        os_error_type actual_code {0};
        std::vector<SocketHost> hosts;

        auto it {std::back_inserter(hosts)};
        if (const auto result {insert(it, host, {}, hints, is_verbose)}) {
            if (is_verbose || expected_codes == ErrorCodeSet {0}) {
                print(result);
            }

            actual_code = result.number();
        } else {
            print(hosts);
        }

        assert(expected_codes.contains(actual_code));
    }

    auto test_invalid_family() -> void
    {
        const SocketHints hints {-1, SOCK_STREAM, 0, 0};
        test_host(localhost, hints, get_codes_family());
    }

#ifndef OS_CYGWIN_NT
    auto test_invalid_protocol() -> void
    {
        const SocketHints hints {AF_UNSPEC, SOCK_STREAM, -1, 0};
        test_host(localhost, hints, get_codes_protocol());
    }
#endif

    auto test_invalid_socktype() -> void
    {
        const SocketHints hints {AF_UNSPEC, -1, 0, 0};
        test_host(localhost, hints, get_codes_socktype());
    }

    auto test_valid(const HostnameView& host) -> void
    {
        if (host.empty()) {
            std::cout << "Local host:"
                      << std::endl;
        }
        else {
            std::cout << "Host: "
                      << host
                      << std::endl;
        }

        test_host(host, ip_any, {0});
    }
}

auto main(int argc, char* argv[]) -> int
{
    try {
        const auto args {parse_arguments(argc, argv)};
        const auto context {start_context(is_verbose)};

        if (is_verbose) {
            std::cout << *context << std::endl;
        }

        test_invalid_family();
        test_invalid_socktype();
#if !defined(OS_CYGWIN_NT) && !defined(OS_MINGW64_NT)
        test_invalid_protocol();
#endif

        StringList hosts;
        std::ranges::copy(args, std::back_inserter(hosts));

        if (hosts.empty()) {
            hosts.emplace_back(get_hostname());
        }

        std::ranges::for_each(hosts, test_valid);
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
