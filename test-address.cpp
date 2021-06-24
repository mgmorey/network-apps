#include "network-address.h"    // Address, Hostname
#include "network-addresses.h"  // Addresses, get_addresses()
#include "network-endpoint.h"   // to_string()
#include "network-hostname.h"   // get_hostname()
#include "network-socket.h"     // Socket

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, PF_INET, PF_INET6,
                        // IPPROTO_IP, IPPROTO_TCP, IPPROTO_UDP,
                        // SOCK_DGRAM, SOCK_STREAM
#else
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
    const struct addrinfo& ai = static_cast<const struct addrinfo&>(socket);
    return ai.ai_family;
}

static void print_address(const Network::Address& address)
{
    Network::Hostname addr(to_string(address));
    Network::Hostname cname(address.canonical_name());
    Network::Hostname host(to_hostname(address, true));
    std::vector<Network::Hostname> names;

    if (!cname.empty()) {
        names.push_back(cname);
    }

    if (host != addr && host != cname) {
        names.push_back(host);
    }

    std::cout << '\t' << addr << " (";

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

static void print_addresses(const Network::Addresses& addresses,
                            const std::string& desc)
{
    if (addresses.empty()) {
        return;
    }

    std::cout << (desc.empty() ? "All" : desc)
              << " addresses:"
              << std::endl;

    for (Network::Addresses::const_iterator it = addresses.begin();
         it != addresses.end();
         ++it)
    {
        print_address(*it);
    }
}

static void test_host(const Network::Hostname& host,
                      const Network::Socket& hints,
                      bool verbose = true)
{
    Network::AddressesResult addrs_result(get_addresses(host, hints, verbose));
    Network::Addresses addrs(addrs_result.first);
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

        std::cerr << " addresses: "
                  << result.string()
                  << std::endl;
    }
    else if (!addrs.empty()) {
        print_addresses(addrs, desc);
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
