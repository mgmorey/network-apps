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

static std::string get_family_description(int family)
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

static Network::Hostname get_hostname(const Network::Address& address,
                                      int flags = 0)
{
    Network::EndpointResult endpoint_result(address.endpoint(flags));

    if (!endpoint_result.second.second.empty()) {
        std::cerr << endpoint_result.second.second
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

static void print_addresses(const Network::Addresses& addresses, int family)
{
    if (addresses.empty()) {
        return;
    }

    std::string description(get_family_description(family));
    std::cout << (description.empty() ? "All" : description)
              << " addresses:"
              << std::endl;

    for (Network::Addresses::const_iterator it = addresses.begin();
         it != addresses.end();
         ++it)
    {
        Network::Hostname address(get_hostname(*it, NI_NUMERICHOST));

        if (address.empty()) {
            continue;
        }

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

static Network::Addresses test_host(const Network::Hostname& host,
                                    int family, bool print = true)
{
    Network::Socket hints(family);
    Network::AddressesResult result(Network::get_addresses(host, "", hints));
    Network::Addresses addresses(result.first);
    Network::Result addr_result(result.second);
    int code = addr_result.first;

    if (code != 0) {
        std::string description(get_family_description(family));
        std::string error(addr_result.second);

        if (description.empty()) {
            std::cerr << "No";
        }
        else {
            std::cerr << "No "
                      << description;
        }
        
        std::cerr << " addresses: "
                  << error
                  << std::endl;
    }
    else if (!addresses.empty() && print) {
        print_addresses(addresses, family);
    }

    return addresses;
}

static void test_host(const Network::Hostname& host)
{
    if (!host.empty()) {
        std::cout << "Host: " << host << std::endl;
    }

    Network::Addresses ipv4(test_host(host, AF_INET));
    Network::Addresses ipv6(test_host(host, AF_INET6));
    Network::Addresses any(test_host(host, AF_UNSPEC, false));
    std::set<Network::Address> set_all;
    std::set<Network::Address> set_any;
    insert(ipv4, set_all);
    insert(ipv6, set_all);
    insert(any, set_any);

    if (set_any != set_all) {
        print_addresses(any, AF_UNSPEC);
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
