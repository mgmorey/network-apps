#include "network-endpoint.h"   // to_string()
#include "network-host.h"       // Host, Hostname
#include "network-hosts.h"      // Hosts, get_hosts()
#include "network-hostname.h"   // get_hostname()
#include "network-socket.h"     // Socket

#ifdef _WIN32
#include <ws2tcpip.h>   // AI_ADDRCONFIG, AI_ALL, AI_CANONNAME,
#include <winsock2.h>   // AF_INET, AF_INET6, PF_INET, PF_INET6,
                        // IPPROTO_IP, IPPROTO_TCP, IPPROTO_UDP,
                        // SOCK_DGRAM, SOCK_STREAM, WSACleanup(),
                        // WSAStartup()
#else
#include <netdb.h>      // AI_ADDRCONFIG, AI_ALL, AI_CANONNAME,
#include <netinet/in.h> // IPPROTO_IP, IPPROTO_TCP, IPPROTO_UDP
#include <sys/socket.h> // AF_INET, AF_INET6, PF_INET, PF_INET6,
                        // SOCK_DGRAM, SOCK_STREAM
#endif

#include <cstdlib>      // EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>     // std::cerr, std::cout, std::endl
#include <string>       // std::string
#include <vector>       // std::vector

static std::string get_description(int family)
{
    std::string result;

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

static int get_family(const Network::Socket& socket)
{
    const addrinfo& ai = static_cast<const addrinfo&>(socket);
    return ai.ai_family;
}

static void print_host(const Network::Host& host)
{
    std::vector<Network::Hostname> names;
    Network::Hostname cname(host.canonical_name());

    if (!cname.empty()) {
        names.push_back(cname);
    }

    std::string address(to_string(host));
    Network::Hostname hostname(to_hostname(host, true));

    if (hostname != address && hostname != cname) {
        names.push_back(hostname);
    }

    std::cout << '\t' << address << " (";

    for (std::vector<Network::Hostname>::const_iterator it = names.begin();
         it != names.end();
         ++it) {
        if (it != names.begin()) {
            std::cout << ", ";
        }

        std::cout << (*it);
    }

    std::cout << ')' << std::endl;
}

static void print_hosts(const Network::Hosts& hosts,
                        const std::string& desc)
{
    if (hosts.empty()) {
        return;
    }

    std::cout << (desc.empty() ? "All" : desc)
              << " hosts:"
              << std::endl;

    for (Network::Hosts::const_iterator it = hosts.begin();
         it != hosts.end();
         ++it)
    {
        print_host(*it);
    }
}

static void test_host(const Network::Hostname& host,
                      const Network::Socket& hints,
                      bool verbose = true)
{
    Network::HostsResult addrs_result(get_hosts(host, hints, verbose));
    Network::Hosts addrs(addrs_result.first);
    Network::Result result(addrs_result.second);
    int family = get_family(hints);
    std::string desc(get_description(family));

    if (result.nonzero()) {

        if (desc.empty()) {
            std::cerr << "No";
        }
        else {
            std::cerr << "No "
                      << desc;
        }

        std::cerr << " hosts: "
                  << result.string()
                  << std::endl;
    }
    else if (!addrs.empty()) {
        print_hosts(addrs, desc);
    }
}

static void test_host(const Network::Hostname& host)
{
    static const int lflags = AI_ADDRCONFIG | AI_CANONNAME;
    static const int rflags = AI_CANONNAME;

    if (host.empty()) {
        Network::Socket hints(IPPROTO_TCP, SOCK_STREAM, AF_UNSPEC, lflags);
        test_host(host, hints, true);
    }
    else {
        std::cout << "Host: " << host << std::endl;
        Network::Socket hints4(IPPROTO_TCP, SOCK_STREAM, AF_INET, rflags);
        Network::Socket hints6(IPPROTO_TCP, SOCK_STREAM, AF_INET6, rflags);
        test_host(host, hints4, true);
        test_host(host, hints6, true);
    }
}

static int wsa_set_up(void)
{
    int error = 0;

#ifdef WIN32
    WSADATA wsaData;
    error = WSAStartup(MAKEWORD(2, 0), &wsaData);

    if (error != 0) {
        std::cerr << "WSAStartup() returned "
                  << error
                  << std::endl;
    }
#endif

    return error;
}

static void wsa_tear_down(void)
{
#ifdef WIN32
    WSACleanup();
#endif
}

int main(int argc, char* argv[])
{
    Network::Hostname host(argc > 1 ? argv[1] : "example.com");
    int result = EXIT_FAILURE;

    if (wsa_set_up()) {
        goto clean_up;
    }

    if (argc <= 1) {
        test_host("");
    }

    test_host(host);
    result = EXIT_SUCCESS;

clean_up:
    wsa_tear_down();
    return result;
}
