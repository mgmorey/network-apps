// Copyright (C) 2022  "Michael G. Morey" <mgmorey@gmail.com>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "network/network.h"            // Context, ErrorResult,
                                        // OsErrorResult, Overloaded,
                                        // Version, get_hostname()

#ifdef _WIN32
#include <getopt.h>         // getopt(), optarg, opterr, optind
#else
#include <unistd.h>         // getopt(), optarg, opterr, optind
#endif

#include <cassert>      // assert()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <span>         // std::span
#include <vector>       // std::vector

namespace TestContext
{
#ifdef _WIN32
    static const auto expected_error_invalid_version {
        "The Windows Sockets version requested is not supported."
    };
    static const auto expected_result_number {10093};
    static const auto expected_result_string {
        "Call to gethostname(...) failed with error 10093: "
            "Either the application has not called WSAStartup, "
            "or WSAStartup failed."
    };
    static const auto expected_status {"Running"};
    static const auto expected_system {"WinSock 2.0"};
    static const auto expected_version {Network::Version {2, 2}};
    static const auto invalid_version {Network::Version {0, 0}};
#else
    static const auto expected_error_invalid_version {""};
    static const auto expected_result_number {0};
    static const auto expected_result_string {""};
    static const auto expected_status {"Running"};
    static const auto expected_system {
        "Berkeley Software Distribution Sockets"
    };
    static const auto expected_version {Network::Version {0, 0}};
    static const auto invalid_version {Network::Version {0, 0}};
#endif

    static bool verbose {false};  // NOLINT

    static auto parse_arguments(int argc, char** argv) ->
        std::vector<std::string>
    {
        std::vector<std::string> result {*argv};
        int ch {};

        while ((ch = ::getopt(argc, argv, "v")) != -1) {
            switch (ch) {
            case 'v':
                verbose = true;
                break;
            case '?':
                std::cerr << "Usage: "
                          << *argv
                          << " [-v]"
                          << std::endl;
                std::exit(EXIT_FAILURE);
            default:
                abort();
            }
        }

        const auto args = std::span(argv, size_t(argc));

        for (auto index = optind; index < argc; ++index) {
            result.emplace_back(args[index]);
        }

        return result;
    }

    static auto get_hostname_error_result() -> Network::OsErrorResult
    {
        Network::OsErrorResult result;
        const auto hostname_result {Network::get_hostname()};
        std::visit(Network::Overloaded {
                [&](const std::string& hostname) {
                    static_cast<void>(hostname);
                    result = {0, ""};
                },
                [&](const Network::OsErrorResult& error_result) {
                    result = error_result;
                }
            }, hostname_result);
        return result;
    }

    static auto print_error_result(const Network::OsErrorResult& result) -> void
    {
        if (verbose) {
            std::cerr << "Number: "
                      << result.number()
                      << std::endl
                      << "String: "
                      << result.string()
                      << std::endl;
        }
    }

    static auto print_exception(const Network::Error& error) -> void
    {
        if (verbose) {
            std::cerr << "Exception: "
                      << error.what()
                      << std::endl;
        }
    }

    static auto print_strings(const Network::Context& context) -> void
    {
        if (verbose) {
            std::cerr << "Status: "
                      << context.status()
                      << std::endl
                      << "System: "
                      << context.system()
                      << std::endl
                      << "Version: "
                      << context.version()
                      << std::endl;
        }
    }

    static auto test_context(const Network::Context& context) -> void
    {
        print_strings(context);
        assert(context.status() == expected_status);		// NOLINT
        assert(context.system() == expected_system);		// NOLINT
        assert(context.version() == expected_version);		// NOLINT
    }

    static auto test_context_invalid_version() -> void
    {
        std::string what;

        try {
            const Network::Context context {invalid_version};
            static_cast<void>(context);
        }
        catch (Network::Error& error) {
            print_exception(error);
            what = error.what();
        }

        assert(what == expected_error_invalid_version);		// NOLINT
    }

    static auto test_context_valid_version() -> void
    {
        std::string what;

        try {
            const Network::Context context;
            TestContext::test_context(context);
        }
        catch (Network::Error& error) {
            print_exception(error);
            what = error.what();
        }

        assert(what == "");					// NOLINT
    }

    static auto test_hostname_with_context() -> void
    {
        const Network::Context context;
        static_cast<void>(context);
        const auto result {get_hostname_error_result()};
        print_error_result(result);
        assert(result.number() == 0);				// NOLINT
        assert(result.string() == "");				// NOLINT
    }

    static auto test_hostname_without_context() -> void
    {
        const auto result {get_hostname_error_result()};
        print_error_result(result);
        assert(result.number() == expected_result_number);	// NOLINT
        assert(result.string() == expected_result_string);	// NOLINT
    }
}

auto main(int argc, char* argv[]) -> int
{
    try {
        const auto args {TestContext::parse_arguments(argc, argv)};
        TestContext::test_context_valid_version();
        TestContext::test_context_invalid_version();
        TestContext::test_hostname_without_context();
        TestContext::test_hostname_with_context();
    }
    catch (std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
