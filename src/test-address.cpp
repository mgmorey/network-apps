#include "network-address.h"    // Address
#include "network-context.h"    // Context
#include "network-endpoint.h"   // to_endpoint()
#include "network-host.h"       // Host, Hostname, Result
#include "network-hosts.h"      // Hosts, get_hosts()
#include "network-hostname.h"   // get_hostname()
#include "network-socket.h"     // Socket

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, PF_INET, PF_INET6,
                        // IPPROTO_IP, IPPROTO_TCP, IPPROTO_UDP,
                        // SOCK_DGRAM, SOCK_STREAM
#include <ws2tcpip.h>   // AI_ADDRCONFIG, AI_ALL, AI_CANONNAME,
#else
#include <netdb.h>      // AI_ADDRCONFIG, AI_ALL, AI_CANONNAME,
#include <netinet/in.h> // IPPROTO_IP, IPPROTO_TCP, IPPROTO_UDP
#include <sys/socket.h> // AF_INET, AF_INET6, PF_INET, PF_INET6,
                        // SOCK_DGRAM, SOCK_STREAM
#endif

#include <algorithm>    // std::for_each(), std::unique()
#include <cstdlib>      // EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>     // std::cerr, std::cout, std::endl
#include <list>         // std::list
#include <ostream>      // std::ostream
#include <string>       // std::string
#include <vector>       // std::vector

class Comparator
{
public:
    Comparator(const std::string& t_key = "") :
        m_key(t_key)
    {
    }

    bool operator()(const std::string& t_string)
    {
        return t_string == m_key;
    }

private:
    std::string m_key;
};

class Formatter
{
public:
    typedef std::vector<std::string> Values;

    Formatter(std::ostream& t_os) :
        m_os(t_os)
    {
    }

    void operator()(const Network::Host& t_host)
    {
        const Network::Address address(t_host);

        if (address.empty()) {
            std::cerr << "No address for host "
                      << t_host.canonical_name()
                      << std::endl;
            return;
        }

        const Network::EndpointResult
            endpoint_result(address.to_endpoint(false, true));
        const Network::Endpoint endpoint(endpoint_result.first);
        const Network::Result result(endpoint_result.second);

        if (result.nonzero()) {
            std::cerr << result
                      << std::endl;
            return;
        }

        Values values;
        values.push_back(address.text());
        values.push_back(endpoint.first);
        values.push_back(t_host.canonical_name());
        values.erase(std::unique(values.begin(),
                                 values.end()),
                     values.end());
        values.erase(std::remove_if(values.begin(),
                                    values.end(),
                                    Comparator()),
                     values.end());
        print(values);
    }

    void print(const Values& values)
    {
        m_os << '\t';

        for (std::size_t i = 0; i < values.size(); ++i) {
            switch (i) {
            case 0:
                m_os << values[i]
                   << " (";
                break;
            case 1:
                m_os << values[i];
                break;
            default:
                m_os << ", "
                   << values[i];
            }
        }

        if (values.size()) {
            m_os << ')';
        }

        m_os << std::endl;
    }

private:
    std::ostream& m_os;
};

static std::ostream& operator<<(std::ostream& os, const Network::Hosts& hosts)
{
    std::for_each(hosts.begin(), hosts.end(), Formatter(os));
    return os;
}

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
    const Network::HostsResult hosts_result(get_hosts(host, hints, verbose));
    const Network::Hosts hosts(hosts_result.first);
    const Network::Result result(hosts_result.second);
    const std::string desc(get_description(hints));

    if (result.nonzero()) {
        if (desc.empty()) {
            std::cout << "No";
        }
        else {
            std::cout << "No "
                      << desc;
        }

        std::cout << " hosts:"
                  << std::endl
                  << result
                  << std::endl;
    }
    else if (!hosts.empty()) {
        std::cout << (desc.empty() ? "All" : desc)
                  << " hosts:"
                  << std::endl
                  << hosts;
    }
}

static void test_host(const Network::Hostname& host)
{
    static const int lflags = AI_ADDRCONFIG | AI_CANONNAME;
    static const int rflags = AI_CANONNAME;

    if (host.empty()) {
        Network::Socket hints(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP, lflags);
        test_host(host, hints, true);
    }
    else {
        std::cout << "Host: " << host << std::endl;
        Network::Socket hints4(AF_INET, SOCK_STREAM, IPPROTO_TCP, rflags);
        Network::Socket hints6(AF_INET6, SOCK_STREAM, IPPROTO_TCP, rflags);
        test_host(host, hints4, true);
        test_host(host, hints6, true);
    }
}

int main(int argc, char* argv[])
{
    const Network::Context context;
    const Network::Hostname host(argc > 1 ? argv[1] : "example.com");

    if (argc <= 1) {
        test_host("");
    }

    test_host(host);
    static_cast<void>(context);
    return EXIT_SUCCESS;
}
