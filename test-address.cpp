#include "network-address.h"    // Address, Hostname
#include "network-addresses.h"  // Addresses, get_addresses()
#include "network-endpoint.h"   // Endpoint, EndpointResult,
                                // get_hostname()
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
#include <iostream>     // std::cerr, std::cout, std::endl
#include <iterator>     // std::inserter()
#include <set>          // std::set
#include <string>       // std::string

static Network::Hostname get_hostname(const Network::Address& address,
                                      int flags = 0)
{
    Network::EndpointResult endpoint_result(address.endpoint(flags));

    if (!endpoint_result.second.empty()) {
        std::cerr << endpoint_result.second
                  << std::endl;
    }

    return Network::get_hostname(endpoint_result.first);
}

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

    std::cout << family
              << " addresses:"
              << std::endl;

    for (Network::Addresses::const_iterator it = addresses.begin();
         it != addresses.end();
         ++it)
    {
        Network::Hostname address(get_hostname(*it, NI_NUMERICHOST));

        if (!address.empty()) {
            std::cout << '\t'
                      << address;

            Network::Hostname hostname(get_hostname(*it));

            if (hostname != address) {
                std::cout << " ("
                          << hostname
                          << ") ";
            }

            std::cout << std::endl;
        }
    }
}

static void test_host(const Network::Hostname& hostname = "")
{
    if (!hostname.empty()) {
        std::cout << "Host: " << hostname << std::endl;
    }

    Network::Socket hints_ipv4(AF_INET);
    Network::Socket hints_ipv6(AF_INET6);
    Network::Addresses any(Network::get_addresses(hostname));
    Network::Addresses ipv4(Network::get_addresses(hostname, "", hints_ipv4));
    Network::Addresses ipv6(Network::get_addresses(hostname, "", hints_ipv6));
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

int main(int argc, char* argv[])
{
    Network::Hostname hostname(argc > 1 ? argv[1] : "example.com");
    int result = EXIT_FAILURE;

    if (wsa_set_up()) {
        goto clean_up;
    }

    if (argc <= 1) {
        test_host();
    }

    test_host(hostname);
    result = EXIT_SUCCESS;

clean_up:
    wsa_tear_down();
    return result;
}
