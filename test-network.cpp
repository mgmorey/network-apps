#include "network.h"

#include <cstdlib>
#include <iostream>

static void print_addresses(const Addresses& addrs,
                            const std::string& str)
{
    std::cout << str << std::endl << std::endl;

    for (Addresses::const_iterator it = addrs.begin();
         it != addrs.end();
         ++it)
    {
        std::cout << *it << std::endl;
    }

    std::cout << std::endl;
}

static void test_host(const std::string& host = "")
{
    Addresses addrs = get_addresses(host);
    Addresses addrs4 = get_addresses(host, PF_INET);
    Addresses addrs6 = get_addresses(host, PF_INET6);
    std::cout << "Hostname:\t" << host << std::endl << std::endl;
    print_addresses(addrs, "All Addresses:");
    print_addresses(addrs4, "IPv4 Addresses:");
    print_addresses(addrs6, "IPv6 Addresses:");
}

int main(void)
{
    test_host();
    test_host("example.com");
    return EXIT_SUCCESS;
}
