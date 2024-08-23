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
#include "network/cleanup.h"            // cleanup()
#include "network/network.h"            // Context, Error,
                                        // OptionalVersion, Version,
                                        // get_hostname()
#include "network/parse.h"              // parse()

#ifdef WIN32
#include <winsock2.h>       // WSAEFAULT, WSAEPROCLIM,
                            // WSANOTINITIALISED, WSASYSNOTREADY,
                            // WSAVERNOTSUPPORTED
#endif

#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <sstream>      // std::ostringstream
#include <string>       // std::string

namespace TestContext
{
    using Network::Context;
    using Network::Error;
    using Network::Hostname;
    using Network::OptionalVersion;
    using Network::Version;
#ifdef WIN32
    using Network::WindowsVersion;
#endif
    using Network::context_error_type;
    using Network::get_hostname;
    using Network::parse;
    using Network::version_null;

    static constexpr Version version_0_0 {0, 0};
    static constexpr Version version_0_1 {0, 1};
    static constexpr Version version_1_0 {1, 0};
    static constexpr Version version_2_0 {2, 0};
    static constexpr Version version_max {255, 255};

    static_assert(version_0_0 == Version {} && Version {} == Version {0, 0});
    static_assert(version_0_0 != version_0_1 && version_0_1 != version_1_0);
    static_assert(version_0_0 < version_0_1 && version_0_1 < version_1_0);
    static_assert(version_1_0 > version_0_1 && version_1_0 > version_0_0);
    static_assert(version_0_0 < version_max);

#ifdef WIN32
    static_assert(WORD {WindowsVersion(version_0_0)} == 0x0U);        // NOLINT
    static_assert(Version {WindowsVersion(0x0U)} == version_0_0);     // NOLINT
    static_assert(WORD {WindowsVersion(version_0_1)} == 0x100U);      // NOLINT
    static_assert(Version {WindowsVersion(0x100U)} == version_0_1);   // NOLINT
    static_assert(WORD {WindowsVersion(version_1_0)} == 0x1U);	      // NOLINT
    static_assert(Version {WindowsVersion(0x1U)} == version_1_0);     // NOLINT
    static_assert(WORD {WindowsVersion(version_2_0)} == 0x2U);	      // NOLINT
    static_assert(Version {WindowsVersion(0x2U)} == version_2_0);     // NOLINT
    static_assert(WORD {WindowsVersion(version_max)} == 0xFFFFU);     // NOLINT
    static_assert(Version {WindowsVersion(0xFFFFU)} == version_max);  // NOLINT
#endif

#ifdef WIN32
    static constexpr auto expected_code_stopped {WSANOTINITIALISED};
    static constexpr auto expected_context_re {
        R"(WinSock 2.0 Version \d{1,3}\.\d{1,3} Running)"
            };
    static constexpr auto expected_error_stopped {
        "Call to ::gethostname(, 1024) failed with error 10093: "
        "Either the application has not called WSAStartup, "
        "or WSAStartup failed."
    };
    static constexpr auto expected_error_version {
        "The Windows Sockets version requested is not supported."
    };
#else
    static constexpr auto expected_code_stopped {0};
    static constexpr auto expected_context_re {
        R"(Berkeley Software Distribution Sockets( Version \d{1,3}\.\d{1,3})?)"
    };
    static constexpr auto expected_error_stopped {""};
    static constexpr auto expected_error_version {""};
#endif

    static const auto mode {Context::failure_mode::return_error};
    static bool is_verbose {false};  // NOLINT

    class TestContext final :
        public Context
    {
    public:
        static auto test_instance() -> TestContext&
        {
            static TestContext test_context;
            return test_context;
        }

        explicit TestContext(const OptionalVersion& t_version = {}) :
            Context(t_version)
        {
        }

        TestContext(const TestContext&) = delete;
        TestContext(const TestContext&&) = delete;

        ~TestContext() final
        {
            if (is_started() && cleanup(failure_mode::return_zero) == 0) {
                is_started(false);
            }
        }

        auto operator=(const TestContext&) -> TestContext& = delete;
        auto operator=(const TestContext&&) -> TestContext& = delete;
    };

    auto parse_arguments(int argc, char** argv) -> void
    {
        const auto [_, options] {parse(argc, argv, "v")};

        if (options.contains('?')) {
            std::cerr << "Usage: "
                      << *argv
                      << " [-v]"
                      << std::endl;
            std::exit(EXIT_FAILURE);
        }

        if (options.contains('v')) {
            is_verbose = true;
        }

        static_cast<void>(_);
    }

    auto print(const TestContext& context,
               const std::string& scope) -> void
    {
        std::cout << "Context";

        if (is_verbose) {
            std::cout << ' '
                      << &context;
        }

        std::cout << ": "
                  << scope
                  << std::endl
                  << "    "
                  << context
                  << std::endl;
    }

    auto print(const Error& error) -> void
    {
        if (is_verbose) {
            std::cout << "Exception: "
                      << error.what()
                      << std::endl;
        }
    }

    auto test_context(const TestContext& context,
                      const std::string& scope) -> void
    {
        print(context, scope);
        std::ostringstream oss;
        oss << context;
        const std::string actual_context_str {oss.str()};
        const std::regex expected_context_regex {expected_context_re};
        assert(std::regex_match(actual_context_str, expected_context_regex));
    }

    auto test_context_cleaned_up() -> void
    {
        context_error_type error_code {0};
        std::string actual_error_str;

        try {
            error_code = cleanup(mode);
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(error_code == expected_code_stopped);
        assert(actual_error_str.empty());
    }

    auto test_context_global_instance() -> void
    {
        std::string actual_error_str;

        try {
            TestContext& context1 {TestContext::test_instance()};
            TestContext& context2 {TestContext::test_instance()};
            test_context(context1, "global");
            test_context(context2, "global");
            assert(&context1 == &context2);
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str.empty());
        test_context_cleaned_up();
    }

    auto test_context_local_instances() -> void
    {
        std::string actual_error_str;

        try {
            const TestContext context_1 {version_1_0};
            test_context(context_1, "local 1");
            const TestContext context_2 {version_2_0};
            test_context(context_2, "local 2");
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str.empty());
        test_context_cleaned_up();
    }

    auto test_context_valid_with_shutdown() -> void
    {
        std::string actual_error_str;

        try {
            const TestContext context;
            test_context(context, "local 3");
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str.empty());
        test_context_cleaned_up();
    }

    auto test_context_valid_without_shutdown() -> void
    {
        std::string actual_error_str;

        try {
            const TestContext context;
            test_context(context, "local 4");
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str.empty());
        test_context_cleaned_up();
    }

    auto test_context_version_null() -> void
    {
        std::string actual_error_str;

        try {
            const TestContext context {version_null};
            static_cast<void>(context);
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str == expected_error_version);
        test_context_cleaned_up();
    }

    auto test_hostname_running() -> void
    {
        std::string actual_error_str;

        try {
            const TestContext context;
            test_context(context, "local 5");
            static_cast<void>(get_hostname(is_verbose));
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str.empty());
        test_context_cleaned_up();
    }

    auto test_hostname_stopped() -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(get_hostname(is_verbose));
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str == expected_error_stopped);
        test_context_cleaned_up();
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace TestContext;

    try {
        parse_arguments(argc, argv);
        test_context_global_instance();
        test_context_local_instances();
        test_context_valid_with_shutdown();
        test_context_valid_without_shutdown();
        test_context_version_null();
        test_hostname_running();
        test_hostname_stopped();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
