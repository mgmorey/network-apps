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
                                        // Version, get_hostname(),
                                        // version_null
#include "network/assert.h"             // assert()

#ifdef _WIN32
#include <getopt.h>         // getopt(), optarg, opterr, optind
#include <winsock2.h>       // WSAEFAULT, WSAEPROCLIM,
                            // WSANOTINITIALISED, WSASYSNOTREADY,
                            // WSAVERNOTSUPPORTED
#else
#include <unistd.h>         // getopt(), optarg, opterr, optind
#endif

#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <span>         // std::span
#include <vector>       // std::vector

namespace TestContext
{
    static const auto expected_code_initialized {0};
#ifdef _WIN32
    static const auto expected_code_uninitialized {WSANOTINITIALISED};
    static const auto expected_error_invalid_version {
        "The Windows Sockets version requested is not supported."
    };
    static const auto expected_error_uninitialized {
        "Either the application has not called WSAStartup, "
        "or WSAStartup failed."
    };
    static const auto expected_system {"WinSock 2.0 (Test)"};
    static const auto expected_version {Network::Version {2, 2}};
#else
    static const auto expected_code_uninitialized {0};
    static const auto expected_error_invalid_version {""};
    static const auto expected_error_uninitialized {""};
    static const auto expected_system {
        "Berkeley Software Distribution Sockets (Test)"
    };
    static const auto expected_version {Network::Version {}};
#endif
    static const auto invalid_version {Network::Version {0, 0}};

    static bool verbose {false};  // NOLINT

    class Context :
        public Network::Context
    {
    public:
        static auto instance() -> Context&
        {
            static Context g_context;
            return g_context;
        }

        explicit Context(const Network::Version& t_version = {}) :
            Network::Context(t_version)
        {
            system(system() + " (Test)");
        }

        static auto cleanup(bool enable_throw = true) -> Network::os_error_type
        {
            return Network::Context::cleanup(enable_throw);
        }

        auto shutdown() -> void
        {
            Network::Context::shutdown();
        }
    };

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

    static auto get_hostname() -> Network::OsErrorResult
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

    static auto print_result(const Network::OsErrorResult& result,
                             const std::string& description = "") -> void
    {
        if (verbose) {
            std::cerr << "Error result"
                      << (description.empty() ? "" : ": " + description)
                      << std::endl
                      << "    Number: "
                      << result.number()
                      << std::endl
                      << "    String: "
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

    static auto print_context(const Network::Context& context,
                              const std::string& description = "") -> void
    {
        if (verbose) {
            std::cerr << "Context "
                      << &context
                      << (description.empty() ? "" : ": " + description)
                      << std::endl
                      << "    Status: "
                      << context.status()
                      << std::endl
                      << "    System: "
                      << context.system()
                      << std::endl
                      << "    Version: "
                      << context.version()
                      << std::endl;
        }
    }

    static auto test_context(const Context& context,
                             const std::string& description = "",
                             const Network::Version& version = {}) -> void
    {
        print_context(context, (description.empty() ? "" :
                                description + " (test)"));
        assert(context.status() == "Running");			// NOLINT
        assert(context.system() == expected_system);		// NOLINT
        assert(context.version() == (version ? version :	// NOLINT
                                     expected_version));
    }

    static auto test_context_cleaned_up() -> void
    {
        std::string what;

        try {
            TestContext::Context::cleanup(true);
        }
        catch (const Network::Error& error) {
            what = error.what();
        }

        assert(what == expected_error_uninitialized);		// NOLINT
    }

    static auto test_context_global_instance() -> void
    {
        std::string status1;
        std::string status2;
        std::string what;

        try {
            Context& context1 {Context::instance()};
            Context& context2 {Context::instance()};
            test_context(context1, "global");
            test_context(context2, "global");
            assert(&context1 == &context2);			// NOLINT
            assert(context1.started());				// NOLINT
            assert(context2.started());				// NOLINT
            context1.shutdown();
            assert(!context1.started());			// NOLINT
            assert(!context2.started());			// NOLINT
            context2.shutdown();
        }
        catch (const Network::Error& error) {
            print_exception(error);
            what = error.what();
        }

        assert(what == "");					// NOLINT
        test_context_cleaned_up();
    }

    static auto test_context_invalid_version() -> void
    {
        std::string what;

        try {
            const Context context {invalid_version};
            static_cast<void>(context);
        }
        catch (const Network::Error& error) {
            print_exception(error);
            what = error.what();
        }

        assert(what == expected_error_invalid_version);		// NOLINT
        test_context_cleaned_up();
    }

    static auto test_context_local_instances() -> void
    {
        static const Network::Version version1 {1, 0};
        static const Network::Version version2 {2, 0};
        std::string what;

        try {
            Context context1 {version1};
            Context context2 {version2};
            test_context(context1, "local 1", version1);
            test_context(context2, "local 2", version2);
            assert(&context1 != &context2);			// NOLINT
        }
        catch (const Network::Error& error) {
            print_exception(error);
            what = error.what();
        }

        assert(what == "");					// NOLINT
        test_context_cleaned_up();
    }

    static auto test_context_valid_with_shutdown() -> void
    {
        std::string what;

        try {
            Context context;
            test_context(context, "local 3");
            context.shutdown();
        }
        catch (const Network::Error& error) {
            print_exception(error);
            what = error.what();
        }

        assert(what == "");					// NOLINT
        test_context_cleaned_up();
    }

    static auto test_context_valid_without_shutdown() -> void
    {
        std::string what;

        try {
            const Context context;
            test_context(context, "local 4");
        }
        catch (const Network::Error& error) {
            print_exception(error);
            what = error.what();
        }

        assert(what == "");					// NOLINT
        test_context_cleaned_up();
    }

    static auto test_hostname_initialized() -> void
    {
        std::string what;

        try {
            const Context context;
            test_context(context, "local 5");
            const auto result {get_hostname()};
            const auto error_code {result.number()};
            print_result(result, "get_hostname() with context");
            assert(error_code == expected_code_initialized);	// NOLINT
        }
        catch (const Network::Error& error) {
            print_exception(error);
            what = error.what();
        }

        assert(what == "");					// NOLINT
        test_context_cleaned_up();
    }

    static auto test_hostname_uninitialized() -> void
    {
        const auto result {get_hostname()};
        const auto error_code {result.number()};
        print_result(result, "get_hostname() w/o context");
        assert(error_code == expected_code_uninitialized);	// NOLINT
        test_context_cleaned_up();
    }
}

auto main(int argc, char* argv[]) -> int
{
    try {
        const auto args {TestContext::parse_arguments(argc, argv)};
        TestContext::test_context_global_instance();
        TestContext::test_context_invalid_version();
        TestContext::test_context_local_instances();
        TestContext::test_context_valid_with_shutdown();
        TestContext::test_context_valid_without_shutdown();
        TestContext::test_hostname_initialized();
        TestContext::test_hostname_uninitialized();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
