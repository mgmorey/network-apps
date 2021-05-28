#include "network.h"

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

int main(void)
{
    test_host(get_hostname());
    test_host("example.com");
    return EXIT_SUCCESS;
}
