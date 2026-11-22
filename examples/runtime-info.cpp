#include "network/network.hpp"

#include <cstdlib>
#include <iostream>

auto main() -> int
{
    try {
        const auto runtime {Network::run(false)};
        std::cout << "Version: "
                  << runtime->version()
                  << '\n'
                  << "High version: "
                  << runtime->high_version()
                  << '\n'
                  << "Description: "
                  << runtime->description()
                  << '\n'
                  << "System status: "
                  << runtime->system_status()
                  << '\n';
        return EXIT_SUCCESS;
    }
    catch (const Network::Error& error) {
        std::cerr << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
