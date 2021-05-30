#include "network.h"

#ifdef _WIN32
#include <winsock2.h>		// WSACleanup(), WSAStartup()
#endif

#include <cassert>		// assert()
#include <cstdlib>		// EXIT_FAILURE, EXIT_SUCCESS
#include <string>		// std::string
#include <iostream>		// std::cout, std::endl

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
        std::string addr = it->to_string();
        std::string host = it->to_hostname();
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

static void test_host(const std::string& host)
{
    Addresses all = get_addresses(host);
    Addresses ipv4 = get_addresses(host, AF_INET);
    Addresses ipv6 = get_addresses(host, AF_INET6);
    std::cout << "Hostname: " << host << std::endl;
    print_addresses(all, "All");
    print_addresses(ipv4, "IPv4");
    print_addresses(ipv6, "IPv6");
}

static int wsa_set_up(void)
{
    int error = 0;

#ifdef WIN32
    WSADATA wsaData;
    error = WSAStartup(MAKEWORD(2, 0), &wsaData);

    if (error) {
        printf("WSAStartup() returned %d\n", error);
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
    int result = EXIT_FAILURE;

    if (wsa_set_up()) {
        goto clean_up;
    }

    test_host(get_hostname());
    std::cout << std::endl;
    test_host("example.com");
    result = EXIT_SUCCESS;

clean_up:
    wsa_tear_down();
    return result;
}
