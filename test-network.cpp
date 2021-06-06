#include "network-address.h"    // Address
#include "network-addresses.h"  // Addresses, get_addresses()
#include "network-endpoint.h"   // Endpoint, hostname()
#include "network-hostname.h"   // get_hostname()
#include "network-socket.h"     // Socket

#ifdef _WIN32
#include <ws2tcpip.h>   // NI_NUMERICHOST
#include <winsock2.h>   // WSACleanup(), WSAStartup()
#else
#include <netdb.h>      // NI_NUMERICHOST
#endif

#include <algorithm>    // std::copy, std::set_union()
#include <cstdlib>      // EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>     // std::cout, std::endl
#include <iterator>     // std::inserter()
#include <set>          // std::set
#include <string>       // std::string

static void insert(const Network::Addresses& a_list,
                   std::set<Network::Address>& a_set)
{
    std::copy(a_list.begin(), a_list.end(),
              std::inserter(a_set, a_set.begin()));
}

static void print_addresses(const Network::Addresses& addresses,
                            const std::string& family)
{
    if (addresses.empty()) {
        return;
    }

    std::cout << std::endl;
    std::cout << family
              << " addresses:"
              << std::endl
              << std::endl;

    for (Network::Addresses::const_iterator it = addresses.begin();
         it != addresses.end();
         ++it)
    {
        Network::Hostname address(it->hostname(NI_NUMERICHOST));
        Network::Hostname hostname(it->hostname());
        std::cout << '\t'
                  << address;

        if (hostname != address) {
            std::cout << " ("
                      << hostname
                      << ") ";
        }

        std::cout << std::endl;
    }
}

static void test_host(const Network::Hostname& host)
{
    std::cout << "Hostname: " << host << std::endl;
    Network::Socket hints_ipv4(AF_INET);
    Network::Socket hints_ipv6(AF_INET6);
    Network::Addresses any(Network::get_addresses(host));
    Network::Addresses ipv4(Network::get_addresses(host, hints_ipv4));
    Network::Addresses ipv6(Network::get_addresses(host, hints_ipv6));
    std::set<Network::Address> set_all;
    insert(ipv4, set_all);
    insert(ipv6, set_all);
    std::set<Network::Address> set_any;
    insert(any, set_any);

    if (set_any != set_all) {
        print_addresses(any, "Any");
    }

    print_addresses(ipv4, "IPv4");
    print_addresses(ipv6, "IPv6");
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

int main(void)
{
    std::string hostname("example.com");
    int result = EXIT_FAILURE;

    if (wsa_set_up()) {
        goto clean_up;
    }

    test_host(Network::get_hostname());
    std::cout << std::endl;
    test_host(hostname);
    std::cout << std::endl;
    result = EXIT_SUCCESS;

clean_up:
    wsa_tear_down();
    return result;
}
