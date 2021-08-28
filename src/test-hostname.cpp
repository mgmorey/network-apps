#include "network-context.h"    // Context
#include "network-hostname.h"   // Hostname, HostnameResult, Result,
                                // get_hostname()

#include <cstdlib>      // EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>     // std::cerr, std::cout, std::endl

int main(void)
{
    const Network::Context context;
    const auto hostname_result(Network::get_hostname());
    const auto hostname(hostname_result.first);
    const auto result(hostname_result.second);

    if (result.nonzero()) {
        std::cerr << "No hostname: "
                  << result
                  << std::endl;
    }
    else {
        std::cout << "Hostname: "
                  << hostname
                  << std::endl;
    }

    static_cast<void>(context);
    return EXIT_SUCCESS;
}
