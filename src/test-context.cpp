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

#include "network/assert.h"             // assert()
#include "network/network.h"            // Context, ErrorResult,
                                        // OsErrorResult, Overloaded,
                                        // Version, get_hostname()
#include "network/os-features.h"        // WIN32

#ifdef WIN32
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
#ifdef WIN32
    static constexpr auto expected_description {"WinSock 2.0"};
    static constexpr auto expected_error_code_running {0};
    static constexpr auto expected_error_code_stopped {WSANOTINITIALISED};
    static constexpr auto expected_error_stopped {
        "Either the application has not called WSAStartup, "
        "or WSAStartup failed."
    };
    static constexpr auto expected_error_invalid_version {
        "The Windows Sockets version requested is not supported."
    };
    static constexpr auto expected_status {"Running"};
    static constexpr auto expected_version {Network::Version {2, 2}};
#else
    static constexpr auto expected_description {
        "Berkeley Software Distribution Sockets"
    };
    static constexpr auto expected_error_code_running {0};
    static constexpr auto expected_error_code_stopped {0};
    static constexpr auto expected_error_stopped {""};
    static constexpr auto expected_error_invalid_version {""};
    static constexpr auto expected_status {"Running"};
    static constexpr auto expected_version {Network::Version {}};
#endif
    static constexpr auto invalid_version {Network::Version {0}};

    static bool verbose {false};  // NOLINT

    class Context :
        public Network::Context
    {
    public:
        static const auto mode {Network::Context::failure_mode::return_error};

        static auto instance() -> Context&
        {
            static Context context;
            return context;
        }

        explicit Context(const Network::Version& t_version = {}) :
            Network::Context(t_version)
        {
        }

        static auto cleanup() -> Network::os_error_type
        {
            return Network::Context::cleanup(mode);
        }

        auto shutdown() -> void
        {
            Network::Context::shutdown(mode);
        }
    };

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

    static auto print(const Context& context,
                      const std::string& description) -> void
    {
        std::cout << "Context";

        if (verbose) {
            std::cout << ' '
                      << &context;
        }

        std::cout << ": "
                  << description
                  << std::endl
                  << "    Description: "
                  << context.description()
                  << std::endl
                  << "    Status: "
                  << context.system_status()
                  << std::endl
                  << "    Version: "
                  << context.version()
                  << std::endl;
    }

    static auto print(const Network::Error& error) -> void
    {
        if (verbose) {
            std::cout << "Exception: "
                      << error.what()
                      << std::endl;
        }
    }

    static auto print(const Network::OsErrorResult& result,
                      const std::string& description = "") -> void
    {
        if (verbose) {
            std::cout << "Error result"
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

    static auto test_context(const Context& context,
                             const std::string& description = "",
                             Network::Version version = {}) -> void
    {
        if (!static_cast<bool>(version)) {
            version = expected_version;
        }

        print(context, description);
        assert(context.description() == expected_description);
        assert(context.system_status() == expected_status);
        assert(context.version() == version);
    }

    static auto test_context_cleaned_up() -> void
    {
        Network::os_error_type error_code {0};
        std::string what;

        try {
            error_code = TestContext::Context::cleanup();
        }
        catch (const Network::Error& error) {
            print(error);
            what = error.what();
        }

        assert(error_code == expected_error_code_stopped);
        assert(what.empty());
    }

    static auto test_context_global_instance() -> void
    {
        std::string what;

        try {
            Context& context1 {Context::instance()};
            Context& context2 {Context::instance()};
            test_context(context1, "global");
            test_context(context2, "global");
            assert(&context1 == &context2);
            assert(context1.is_started());
            assert(context2.is_started());
            context1.shutdown();
            assert(!context1.is_started());
            assert(!context2.is_started());
            context2.shutdown();
        }
        catch (const Network::Error& error) {
            print(error);
            what = error.what();
        }

        assert(what.empty());
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
            print(error);
            what = error.what();
        }

        assert(what == expected_error_invalid_version);
        test_context_cleaned_up();
    }

    static auto test_context_local_instances() -> void
    {
        constexpr Network::Version version1 {1, 0};
        constexpr Network::Version version2 {2, 0};
        static_assert(version1 < version2);
        static_assert(version2 > version1);
        std::string what;

        try {
            Context context1 {version1};
            Context context2 {version2};
            test_context(context1, "local 1", version1);
            test_context(context2, "local 2", version2);
        }
        catch (const Network::Error& error) {
            print(error);
            what = error.what();
        }

        assert(what.empty());
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
            print(error);
            what = error.what();
        }

        assert(what.empty());
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
            print(error);
            what = error.what();
        }

        assert(what.empty());
        test_context_cleaned_up();
    }

    static auto test_hostname_running() -> void
    {
        std::string what;

        try {
            const Context context;
            test_context(context, "local 5");
            const auto result {get_hostname()};
            const auto error_code {result.number()};
            print(result, "get_hostname() with context");
            assert(error_code == expected_error_code_running);
        }
        catch (const Network::Error& error) {
            print(error);
            what = error.what();
        }

        assert(what.empty());
        test_context_cleaned_up();
    }

    static auto test_hostname_stopped() -> void
    {
        std::string what;

        try {
            static_cast<void>(get_hostname());
        }
        catch (const Network::Error& error) {
            print(error);
            what = error.what();
        }

        assert(what == expected_error_stopped);
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
        TestContext::test_hostname_running();
        TestContext::test_hostname_stopped();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
