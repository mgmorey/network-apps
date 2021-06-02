#include "network-address.h"	// Address
#include "network-addresses.h"	// Addresses, get_addresses()
#include "network-hostname.h"	// get_hostname()
#include "network-socket.h"	// Socket

#ifdef _WIN32
#include <winsock2.h>		// IPPROTO_TCP, WSACleanup(), WSAStartup()
#else
#include <netinet/in.h>		// IPPROTO_TCP
#endif

#include <algorithm>		// std::copy, std::set_union()
#include <cstdlib>		// EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>		// std::cout, std::endl
#include <iterator>		// std::inserter()
#include <set>			// std::set
#include <string>		// std::string

static void insert(const Addresses& a_list, std::set<Address>& a_set)
{
    std::copy(a_list.begin(), a_list.end(),
              std::inserter(a_set, a_set.begin()));
}

static void print_addresses(const Addresses& addresses,
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

    for (Addresses::const_iterator it = addresses.begin();
         it != addresses.end();
         ++it)
    {
        std::string addr(it->to_string());
        Hostname host(it->to_hostname());
        std::cout << '\t'
                  << addr;

        if (host != addr) {
            std::cout << " ("
                      << host
                      << ") ";
        }

        std::cout << std::endl;
    }
}

static void test_host(const Hostname& host)
{
    std::cout << "Hostname: " << host << std::endl;
    Addresses any(get_addresses(host));
    Addresses ipv4(get_addresses(host, Socket(AF_INET)));
    Addresses ipv6(get_addresses(host, Socket(AF_INET6)));
    std::set<Address> set_all;
    insert(ipv4, set_all);
    insert(ipv6, set_all);
    std::set<Address> set_any;
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

    test_host(get_hostname());
    std::cout << std::endl;
    test_host(hostname);
    std::cout << std::endl;
    result = EXIT_SUCCESS;

clean_up:
    wsa_tear_down();
    return result;
}
