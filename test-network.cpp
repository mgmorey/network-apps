#include "network.h"

#ifdef _WIN32
#include <winsock2.h>		// WSACleanup(), WSAStartup()
#endif

#include <cstdlib>
#include <iostream>

static void print_addresses(const Addresses& addrs,
                            const std::string& str)
{
    if (addrs.empty()) {
        return;
    }

    std::cout << str;

    for (Addresses::const_iterator it = addrs.begin();
         it != addrs.end();
         ++it)
    {
        if (it != addrs.begin()) {
            std::cout << ' ';
        }

        std::cout << it->second;
    }

    std::cout << std::endl;
}

static void test_host(const std::string& host)
{
    Addresses addrs = get_addresses(host);
    Addresses addrs4 = get_addresses(host, PF_INET);
    Addresses addrs6 = get_addresses(host, PF_INET6);
    std::cout << "Hostname: " << host << std::endl;
    print_addresses(addrs, "All Addresses: ");
    print_addresses(addrs4, "IPv4 Addresses: ");
    print_addresses(addrs6, "IPv6 Addresses: ");
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
    test_host("example.com");
    result = EXIT_SUCCESS;

clean_up:
    wsa_tear_down();
    return result;
}
