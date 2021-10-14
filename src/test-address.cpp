#include "network-address.h"    // Address
#include "network-addrinfo.h"   // AddrInfo
#include "network-context.h"    // Context
#include "network-endpoint.h"   // Endpoint, EndpointResult, Hostname,
                                // Result, SockAddr, get_endpoint()
#include "network-hints.h"      // Hints
#include "network-host.h"       // Host
#include "network-hostname.h"   // get_hostname()
#include "network-sockaddr.h"   // is_valid()

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
#include <cassert>      // assert()
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <ostream>      // std::ostream
#include <string>       // std::string
#include <utility>      // std::pair
#include <vector>       // std::vector

namespace TestAddress
{
    using Hosts = std::vector<Network::Host>;
    using HostsResult = std::pair<Hosts, Network::Result>;

    static constexpr auto HOST {"example.com"};

    static bool verbose {false};

    template<typename T, typename U>
    void erase(T& c, const U& value)
    {
        c.erase(std::remove(c.begin(),
                            c.end(),
                            value),
                c.end());
    }

    template<typename T>
    void unique(T& c)
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

        void operator()(const Network::Host& t_host)
        {
            const auto address {t_host.address()};

            if (Network::is_valid(address, verbose)) {
                const auto endpoint_result {get_endpoint(address, false, verbose)};
                const auto endpoint {endpoint_result.first};
                const auto result {endpoint_result.second};

                if (result.result() != 0) {
                    std::cerr << result
                              << std::endl;
                    return;
                }

                Values values = {
                    address.text(),
                    endpoint.first,
                    Network::Hostname(t_host.canonical_name())
                };
                erase(values, "");
                unique(values);

                if (values.empty()) {
                    return;
                }

                print(values);
            }
        }

        void print(const Values& values)
        {
            m_os << "    ";

            for (std::size_t i = 0; i < values.size(); ++i) {
                if (i > 1) {
                    m_os << ", ";
                }

                m_os << values[i];

                if (!i) {
                    m_os << " (";
                }
            }

            m_os << ')'
                 << std::endl;
        }

    private:
        std::ostream& m_os;
    };

    static std::string get_description(const Network::Hints& hints)
    {
        std::string result;
        const auto family {static_cast<int>(hints.family())};

        switch (family) {
        case AF_INET:
            result = "IPv4";
            break;
        case AF_INET6:
            result = "IPv6";
            break;
        }

        return result;
    }

    static HostsResult get_hosts(const Network::Hostname& hostname,
                                 const Network::Hints* hints)
    {
        const auto host {Network::get_hostname(hostname)};
        const auto endpoint {Network::Endpoint(host, nullptr)};
        return Network::AddrInfo::get<HostsResult>(endpoint, hints, verbose);
    }

    static std::vector<std::string> parse_arguments(int argc, char** argv)
    {
        std::vector<std::string> args {argv[0]};
        int ch {};

        while ((ch = ::getopt(argc, argv, "v")) != -1) {
            switch (ch) {
            case 'v':
                verbose = true;
                break;
            case '?':
                std::cerr << "Usage: "
                          << argv[0]
                          << " [-v]"
                          << std::endl;
                std::exit(EXIT_FAILURE);
            default:
                abort();
            }
        }

        for (auto index = optind; index < argc; ++index) {
            args.push_back(argv[index]);
        }

        return args;
    }

    static void test_host(const Network::Hostname& host,
                          const Network::Hints& hints)
    {
        const auto description {get_description(hints)};
        const auto hosts_result {get_hosts(host, &hints)};
        const auto hosts {hosts_result.first};
        const auto result {hosts_result.second};

        if (result.result() != 0) {
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
            return;
        }
        else if (!hosts.empty()) {
            if (description.empty()) {
                std::cout << "All";
            }
            else {
                std::cout << description;
            }

            std::cout << " hosts:"
                      << std::endl;
            std::for_each(hosts.begin(), hosts.end(), Test(std::cout));
        }
    }

    static void test_host(const Network::Hostname& host)
    {
        if (host.null() || host.empty()) {
            const auto flags {AI_ADDRCONFIG | AI_CANONNAME};
            Network::Hints hints(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP, flags);
            test_host(host, hints);
        }
        else {
            const auto flags {AI_CANONNAME};
            std::cout << "Host: " << host << std::endl;
            Network::Hints hints4(AF_INET, SOCK_STREAM, IPPROTO_TCP, flags);
            Network::Hints hints6(AF_INET6, SOCK_STREAM, IPPROTO_TCP, flags);
            test_host(host, hints4);
            test_host(host, hints6);
        }
    }
}

int main(int argc, char* argv[])
{
    const auto args {TestAddress::parse_arguments(argc, argv)};
    const Network::Context context(TestAddress::verbose);

    if (context.result().result() != 0) {
        std::cerr << context.result()
                  << std::endl;
    }
    else {
        const auto host(args.size() > 1 ? args[1] : TestAddress::HOST);

        if (args.size() <= 1) {
            TestAddress::test_host("");
        }

        TestAddress::test_host(host);
    }

    static_cast<void>(context);
}
