#include "network/argumentspan.hpp"
#include "network/network.hpp"

#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

auto main(int argc, char* argv[]) -> int
{
    const Network::ArgumentSpan arguments {
        argv,
        static_cast<std::size_t>(argc)
    };
    const std::string hostname {
        arguments.size() > 1 ? arguments.subspan(1).front() : "localhost"
    };

    try {
        const Network::IpSocketHints hints {SOCK_STREAM, AI_CANONNAME};
        std::vector<Network::SocketHost> hosts;

        if (const auto error {
                Network::insert(std::back_inserter(hosts),
                                hostname,
                                {},
                                hints,
                                false),})
        {
            std::cerr << error.string() << '\n';
            return EXIT_FAILURE;
        }

        for (const auto& host : hosts) {
            const Network::Address address {host.address()};
            std::cout << address.text()
                      << " (" << Network::SocketFamily(address.family())
                      << ')';

            if (const auto& canonical {host.canonical_name()}) {
                std::cout << " " << *canonical;
            }

            std::cout << '\n';
        }

        return EXIT_SUCCESS;
    }
    catch (const Network::Error& error) {
        std::cerr << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
