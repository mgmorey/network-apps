#include "network.h"

#include <cstdlib>
#include <iostream>

int main(void)
{
    Value host = getHostname();
    Values addrs4 = getAddresses(PF_INET);
    Values addrs6 = getAddresses(PF_INET6);
    std::cout << "Hostname:\t" << host << std::endl;
    std::cout << std::endl << "IPv4 Addresses:" << std::endl << std::endl;

    for (Values::const_iterator it = addrs4.begin(); it != addrs4.end(); ++it)
    {
        std::cout << *it << std::endl;
    }

    std::cout << std::endl << "IPv6 Addresses:" << std::endl << std::endl;

    for (Values::const_iterator it = addrs6.begin(); it != addrs6.end(); ++it)
    {
        std::cout << *it << std::endl;
    }

    return EXIT_SUCCESS;
}
