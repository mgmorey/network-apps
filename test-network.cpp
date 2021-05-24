#include "network.h"

#include <cstdlib>
#include <iostream>

int main(void)
{
    Value host = getHostname();
    Values addrs = getAddresses(host);
    std::cout << host << std::endl;

    for (Values::const_iterator it = addrs.begin(); it < addrs.end(); ++it)
    {
        std::cout << *it << std::endl;
    }

    return EXIT_SUCCESS;
}
