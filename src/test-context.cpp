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
                                        // get_hostname()

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
    constexpr auto version_invalid {0};

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

    static auto print_strings() -> void
    {
        if (verbose) {
            std::cerr << "Status: "
                      << Network::Context::status_string()
                      << std::endl
                      << "System: "
                      << Network::Context::system_string()
                      << std::endl
                      << "Version: "
                      << Network::Context::version_string()
                      << std::endl;
        }
    }

    static auto test_context_strings() -> void
    {
        print_strings();
        assert(Network::Context::status_string() == "");	// NOLINT
        assert(Network::Context::system_string() == "");	// NOLINT
        assert(Network::Context::version_string() == "0.0");	// NOLINT
    }

    static auto test_context_version_invalid() -> void
    {
        constexpr auto error_version_not_supported {
            "The Windows Sockets version requested is not supported."
        };
        std::string what;

        try {
            const Network::Context context {version_invalid};
            static_cast<void>(context);
        }
        catch (Network::Error& error) {
            print_exception(error);
            what = error.what();
        }

        assert(what == error_version_not_supported);		// NOLINT
    }

    static auto test_context_version_valid() -> void
    {
        constexpr auto status_string {"Running"};
#ifdef _WIN32
        constexpr auto system_string {"WinSock 2.0"};
        constexpr auto version_string {"2.2"};
#else
        constexpr auto system_string {""};
        constexpr auto version_string {"0.0"};
#endif
        std::string what;

        try {
            const Network::Context context;
            assert(context.status_string() == status_string);	// NOLINT
            assert(context.system_string() == system_string);	// NOLINT
            assert(context.version_string() == version_string);	// NOLINT
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
#ifdef _WIN32
        constexpr auto number {10093};
        constexpr auto string {
            "Call to gethostname(...) failed with error 10093: "
            "Either the application has not called WSAStartup, "
            "or WSAStartup failed."
        };
#else
        constexpr auto number {0};
        constexpr auto string {""};
#endif
        const auto result {get_hostname_error_result()};
        print_error_result(result);
        assert(result.number() == number);			// NOLINT
        assert(result.string() == string);			// NOLINT
    }
}

auto main(int argc, char* argv[]) -> int
{
    try {
        const auto args {TestContext::parse_arguments(argc, argv)};
        TestContext::test_context_strings();
        TestContext::test_context_version_valid();
        TestContext::test_context_version_invalid();
        TestContext::test_hostname_without_context();
        TestContext::test_hostname_with_context();
    }
    catch (std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
