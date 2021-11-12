#include "network-context.h"    // Context
#include "network-hostname.h"   // Hostname, HostnameResult, Result,
                                // get_hostname()

#include <iostream>     // std::cerr, std::cout, std::endl
#include <variant>      // std::get(), std::holds_alternative()
#include <vector>       // std::vector

#ifdef _WIN32
#include <getopt.h>     // getopt(), optarg, opterr, optind, optopt
#else
#include <unistd.h>     // getopt(), optarg, opterr, optind, optopt
#endif

namespace TestHostname
{
    static bool verbose {false};

    static std::vector<std::string> parse_arguments(int argc, char** argv)
    {
        std::vector<std::string> args {argv[0]};
        int ch {};

        while ((ch = ::getopt(argc, argv, "v")) != -1) {
            switch (ch) {
            case 'v':
                verbose = true;
                break;
            case '?':
                std::cerr << "Usage: "
                          << argv[0]
                          << " [-v]"
                          << std::endl;
                std::exit(EXIT_FAILURE);
            default:
                abort();
            }
        }

        for (auto index = optind; index < argc; ++index) {
            args.push_back(argv[index]);
        }

        return args;
    }

    static void test_hostname()
    {
        const auto hostname_result {Network::get_hostname()};

        if (std::holds_alternative<Network::Result>(hostname_result)) {
            const auto result {std::get<Network::Result>(hostname_result)};
            std::cerr << "No hostname available: "
                      << result
                      << std::endl;
        }
        else {
            const auto hostname = std::get<Network::Hostname>(hostname_result);
            std::cout << "Hostname: "
                      << hostname
                      << std::endl;
        }
    }
}

int main(int argc, char* argv[])
{
    const auto args {TestHostname::parse_arguments(argc, argv)};
    const Network::Context context(TestHostname::verbose);

    if (context.result()) {
        std::cerr << context.result()
                  << std::endl;
    }
    else {
        TestHostname::test_hostname();
    }

    static_cast<void>(context);
}
