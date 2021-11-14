#include "network/network.h"    // Address, AddrInfo, Hints, Hostname,
                                // Overload, Service, get_endpoint(),
                                // get_hostname()

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
#include <variant>      // std::get(), std::holds_alternative(),
                        // std::variant
#include <vector>       // std::vector

namespace TestAddress
{
    using Hosts = std::vector<Network::Host>;
    using HostsResult = std::variant<Hosts, Network::Result>;

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
            const Network::Address address {t_host.address()};
            const auto endpoint_result {get_endpoint(address)};
            std::visit(Network::Overload {
                    [&](const Network::Endpoint& endpoint) {
                        Values values = {
                            address.text(),
                            endpoint.first,
                            Network::Hostname(t_host.canonical_name())
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

        Network::EndpointResult get_endpoint(const Network::SockAddr& addr)
        {
            return Network::get_endpoint(addr, 0, verbose);
        }

        void print(const Values& values)
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
        switch (hints.family()) {
        case AF_INET:
            return "IPv4";
        case AF_INET6:
            return "IPv6";
        default:
            return "";
        }
    }

    static HostsResult get_hosts(const Network::Hostname& node,
                                 const Network::Service& serv,
                                 const Network::Hints* hints)
    {
        Hosts hosts;
        const auto result {Network::AddrInfo::insert(node, serv, hints, verbose,
                                                     std::back_inserter(hosts))};

        if (result) {
            return result;
        }
        else {
            return hosts;
        }
    }

    static HostsResult get_hosts(const Network::Hostname& hostname,
                                 const Network::Hints* hints)
    {
        HostsResult hosts_result;
        const auto hostname_result {Network::get_hostname(hostname)};
        std::visit(Network::Overload {
                [&](const std::string& host) {
                    const auto endp {Network::Endpoint(host, nullptr)};
                    hosts_result = get_hosts(host, endp.second, hints);
                },
                [&](const Network::Result& result) {
                    hosts_result = result;
                }
            }, hostname_result);
        return hosts_result;
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
        std::visit(Network::Overload {
                [&](const Hosts& hosts) {
                    if (!hosts.empty()) {
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
                    }
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

    static void test_host(const Network::Hostname& host)
    {
        if (host.null()) {
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

    if (context.result()) {
        std::cerr << context.result()
                  << std::endl;
    }
    else {
        const auto host(args.size() > 1 ? args[1] : TestAddress::HOST);

        if (args.size() <= 1) {
            TestAddress::test_host(nullptr);
        }

        TestAddress::test_host(host);
    }

    static_cast<void>(context);
}
