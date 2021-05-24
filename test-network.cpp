#include "network.h"

#include <cstdlib>
#include <iostream>

static void printAddresses(const Addresses& addrs,
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

int main(void)
{
    Address host = getHostname();
    Addresses addrs = getAddresses();
    Addresses addrs4 = getAddresses(PF_INET);
    Addresses addrs6 = getAddresses(PF_INET6);
    std::cout << "Hostname:\t" << host << std::endl << std::endl;
    printAddresses(addrs, "All Addresses:");
    printAddresses(addrs4, "IPv4 Addresses:");
    printAddresses(addrs6, "IPv6 Addresses:");
    return EXIT_SUCCESS;
}
