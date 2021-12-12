#include "network/network.h"        // get_hostname()

#ifdef _WIN32
#include <getopt.h>     // getopt(), optarg, opterr, optind, optopt
#else
#include <unistd.h>     // getopt(), optarg, opterr, optind, optopt
#endif

#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <variant>      // std::visit()
#include <vector>       // std::vector

namespace TestHostname
{
    static bool verbose {false};  // NOLINT

    static auto parse_arguments(int argc, char** argv) ->
        std::vector<std::string>
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
            args.emplace_back(argv[index]);
        }

        return args;
    }

    static auto test_hostname() -> void
    {
        const auto hostname_result {Network::get_hostname()};
        std::visit(Network::Overload {
                [&](const std::string& hostname) {
                    std::cout << "Hostname: "
                              << hostname
                              << std::endl;
                },
                [&](const Network::Result& result) {
                    std::cerr << "No hostname available: "
                              << result
                              << std::endl;
                }
            }, hostname_result);
    }
}

auto main(int argc, char* argv[]) -> int
{
    try {
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
    catch (std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
