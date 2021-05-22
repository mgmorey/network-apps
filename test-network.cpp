#include "network.h"

#include <cstdlib>
#include <iostream>

int main(void)
{
    Value host = getHostName();
    Values addrs = getIpAddress(host);
    std::cout << host << std::endl;

    for (Values::const_iterator it = addrs.begin(); it < addrs.end(); ++it)
    {
        std::cout << *it << std::endl;
    }

    return EXIT_SUCCESS;
}
