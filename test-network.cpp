#include "network.h"

#include <cstdlib>
#include <iostream>

int main(void)
{
    Address host = getHostname();
    Addresses addrs = getAddresses();
    Addresses addrs4 = getAddresses(PF_INET);
    Addresses addrs6 = getAddresses(PF_INET6);
    std::cout << "Hostname:\t" << host << std::endl;
    std::cout << std::endl << "All Addresses:" << std::endl << std::endl;

    for (Addresses::const_iterator it = addrs.begin(); it != addrs.end(); ++it)
    {
        std::cout << *it << std::endl;
    }

    std::cout << std::endl << "IPv4 Addresses:" << std::endl << std::endl;

    for (Addresses::const_iterator it = addrs4.begin(); it != addrs4.end(); ++it)
    {
        std::cout << *it << std::endl;
    }

    std::cout << std::endl << "IPv6 Addresses:" << std::endl << std::endl;

    for (Addresses::const_iterator it = addrs6.begin(); it != addrs6.end(); ++it)
    {
        std::cout << *it << std::endl;
    }

    return EXIT_SUCCESS;
}
