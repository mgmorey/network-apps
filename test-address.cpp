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

static void print_addresses(const Network::Addresses& addresses, int family)
{
    std::string description = get_family_description(family);

    if (addresses.empty()) {
        return;
    }

    if (description.empty()) {
        std::cout << "All";
    }
    else {
        std::cout << description;
    }

    std::cout << " addresses:"
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
    std::string error;
    Network::Socket hints(family);
    Network::Addresses addresses(Network::get_addresses(error, host, "", hints));

    if (!error.empty()) {
        std::cerr << error
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
