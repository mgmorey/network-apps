#include "network-address.h"    // Address
#include "network-context.h"    // Context
#include "network-endpoint.h"   // Endpoint, EndpointResult,
                                // to_endpoint()
#include "network-host.h"       // Host
#include "network-hosts.h"      // Hosts, HostsResult, get_hosts()
#include "network-hostname.h"   // get_hostname()
#include "network-result.h"     // Result
#include "network-socket.h"     // Socket
#include "network-types.h"      // Hostname, Service

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, AF_UNSPEC, IPPROTO_TCP,
                        // SOCK_STREAM
#include <ws2tcpip.h>   // AI_ADDRCONFIG, AI_CANONNAME,
#else
#include <netdb.h>      // AI_ADDRCONFIG, AI_CANONNAME,
#include <netinet/in.h> // IPPROTO_TCP
#include <sys/socket.h> // AF_INET, AF_INET6, AF_UNSPEC, SOCK_STREAM
#endif

#include <algorithm>    // std::for_each(), std::remove_if(),
                        // std::unique()
#include <cstdlib>      // EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>     // std::cerr, std::cout, std::endl
#include <ostream>      // std::ostream
#include <string>       // std::string
#include <vector>       // std::vector

namespace TestAddress
{
    template<typename Container>
    class Empty
    {
    public:
        bool operator()(const Container& t_value)
        {
            return t_value.empty();
        }
    };

    template<typename Container, typename Functor>
    void remove_if(Container& cont, Functor func)
    {
        cont.erase(std::remove_if(cont.begin(), cont.end(), func), cont.end());
    }

    template<typename Container>
    void unique(Container& cont)
    {
        cont.erase(std::unique(cont.begin(), cont.end()), cont.end());
    }

    class Test
    {
    public:
        typedef std::vector<std::string> Values;

        explicit Test(std::ostream& t_os) :
            m_os(t_os)
        {
        }

        void operator()(const Network::Host& t_host)
        {
            const auto address(static_cast<Network::Address>(t_host));
            const auto endpoint_result(address.to_endpoint(false, true));
            const auto endpoint(endpoint_result.first);
            const auto result(endpoint_result.second);

            if (result.nonzero()) {
                std::cerr << result
                          << std::endl;
                return;
            }

            Values values = {
                address.text(),
                endpoint.first,
                t_host.canonical_name()
            };
            remove_if(values, Empty<std::string>());
            unique(values);

            if (values.empty()) {
                return;
            }

            print(values);
        }

        void print(const Values& values)
        {
            m_os << '\t';

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

    static std::string get_description(const Network::Socket& hints)
    {
        std::string result;

        switch (hints.ai_family) {
        case AF_INET:
            result = "IPv4";
            break;
        case AF_INET6:
            result = "IPv6";
            break;
        }

        return result;
    }

    static void test_host(const Network::Hostname& host,
                          const Network::Socket& hints,
                          bool verbose = true)
    {
        const auto description(get_description(hints));
        const auto hosts_result(get_hosts(host, hints, verbose));
        const auto hosts(hosts_result.first);
        const auto result(hosts_result.second);

        if (result.nonzero()) {
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
                std::cout << "All "
                          << description;
            }

            std::cout << " hosts:"
                      << std::endl;
            std::for_each(hosts.begin(), hosts.end(), Test(std::cout));
        }
    }

    static void test_host(const Network::Hostname& host)
    {
        const auto flags = (host.empty() ?
                            AI_ADDRCONFIG | AI_CANONNAME :
                            AI_CANONNAME);

        if (host.empty()) {
            Network::Socket hints(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP, flags);
            test_host(host, hints, true);
        }
        else {
            std::cout << "Host: " << host << std::endl;
            Network::Socket hints4(AF_INET, SOCK_STREAM, IPPROTO_TCP, flags);
            Network::Socket hints6(AF_INET6, SOCK_STREAM, IPPROTO_TCP, flags);
            test_host(host, hints4, true);
            test_host(host, hints6, true);
        }
    }
}

int main(int argc, char* argv[])
{
    const Network::Context context;
    const auto host(argc > 1 ? argv[1] : "example.com");

    if (argc <= 1) {
        TestAddress::test_host("");
    }

    TestAddress::test_host(host);
    static_cast<void>(context);
    return EXIT_SUCCESS;
}
