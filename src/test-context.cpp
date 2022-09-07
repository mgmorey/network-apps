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
#include "network/startup.h"            // to_integer(), to_version()

#ifdef WIN32
#include <getopt.h>         // getopt(), optarg, opterr, optind
#include <winsock2.h>       // WSAEFAULT, WSAEPROCLIM,
                            // WSANOTINITIALISED, WSASYSNOTREADY,
                            // WSAVERNOTSUPPORTED
#else
#include <unistd.h>         // getopt(), optarg, opterr, optind
#endif

#include <cstdlib>      // EXIT_FAILURE, std::exit(), std::size_t
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <span>         // std::span
#include <vector>       // std::vector

using Network::Error;
using Network::Hostname;
using Network::OptionalVersion;
using Network::Version;
using Network::get_hostname;
using Network::os_error_type;
#ifdef WIN32
using Network::to_integer;
#endif

namespace TestContext
{
    static constexpr Network::Version version_0_0 {0, 0};
    static constexpr Network::Version version_0_1 {0, 1};
    static constexpr Network::Version version_1_0 {1, 0};
    static constexpr Network::Version version_2_0 {2, 0};

    static_assert(version_0_0 == version_0_0);
    static_assert(version_0_0 != version_0_1);
    static_assert(version_0_0 != version_1_0);
    static_assert(version_0_1 != version_1_0);
    static_assert(version_0_0 < version_0_1 && version_0_1 < version_1_0);
    static_assert(version_1_0 > version_0_1 && version_1_0 > version_0_0);

#ifdef WIN32
    static_assert(Network::to_integer(version_0_0) == 0x0U);
    static_assert(Network::to_version(0x0U) == version_0_0);
    static_assert(Network::to_integer(version_0_1) == 0x100U);
    static_assert(Network::to_version(0x100U) == version_0_1);
    static_assert(Network::to_integer(version_1_0) == 0x001U);
    static_assert(Network::to_version(0x001U) == version_1_0);
    static_assert(Network::to_integer(version_2_0) == 0x002U);
    static_assert(Network::to_version(0x002U) == version_2_0);
#endif
#ifdef WIN32
    static constexpr auto expected_code_stopped {WSANOTINITIALISED};
    static constexpr auto expected_description {"WinSock 2.0"};
    static constexpr auto expected_error_invalid_version {
        "The Windows Sockets version requested is not supported."
    };
    static constexpr auto expected_error_stopped {
        "Call to ::gethostname(, 1024) failed with error 10093: "
        "Either the application has not called WSAStartup, "
        "or WSAStartup failed."
    };
    static constexpr auto expected_status {"Running"};
    static constexpr auto expected_version {Version {2, 2}};
#else
    static constexpr auto expected_code_stopped {0};
    static constexpr auto expected_description {
        "Berkeley Software Distribution Sockets"
    };
    static constexpr auto expected_error_invalid_version {""};
    static constexpr auto expected_error_stopped {""};
    static constexpr auto expected_status {""};
    static constexpr auto expected_version {Version {}};
#endif
    static constexpr auto invalid_version {Version {0, 0}};

    static const auto mode {Network::Context::failure_mode::return_error};
    static bool verbose {false};  // NOLINT

    class Context :
        public Network::Context
    {
    public:
        static auto instance() -> Context&
        {
            static Context context;
            return context;
        }

        explicit Context(const OptionalVersion& t_version = {}) :
            Network::Context(t_version)
        {
        }

        auto shutdown() -> void
        {
            Network::Context::shutdown(mode);
        }
    };

    static auto parse_arguments(int argc, char** argv) ->
        std::vector<std::string>
    {
        std::vector<std::string> result {*argv};
        int opt {};

        while ((opt = ::getopt(argc, argv, "v")) != -1) {
            switch (opt) {
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

        const auto args = std::span(argv, std::size_t(argc));

        for (auto index = optind; index < argc; ++index) {
            const auto i {static_cast<std::size_t>(index)};
            result.emplace_back(args[i]);
        }

        return result;
    }

    static auto print(const Context& context,
                      const std::string& scope) -> void
    {
        std::cout << "Context";

        if (verbose) {
            std::cout << ' '
                      << &context;
        }

        std::cout << ": "
                  << scope
                  << std::endl
                  << "    Description: "
                  << context.description()
                  << std::endl;
        const auto status {context.system_status()};

        if (!status.empty()) {
            std::cout << "    Status: "
                      << status
                      << std::endl;
        }

        std::cout << "    Version: "
                  << context.version()
                  << std::endl;
    }

    static auto print(const Error& error) -> void
    {
        if (verbose) {
            std::cout << "Exception: "
                      << error.what()
                      << std::endl;
        }
    }

    static auto test_context(const Context& context,
                             const std::string& scope = "",
                             OptionalVersion version = {}) -> void
    {
        print(context, scope);
        assert(context.description() == expected_description);
        assert(context.system_status() == expected_status);
        assert(context.version() == version.value_or(expected_version));
    }

    static auto test_context_cleaned_up() -> void
    {
        os_error_type error_code {0};
        std::string actual_error;

        try {
            error_code = cleanup(mode);
        }
        catch (const Error& error) {
            print(error);
            actual_error = error.what();
        }

        assert(error_code == expected_code_stopped);
        assert(actual_error.empty());
    }

    static auto test_context_global_instance() -> void
    {
        std::string actual_error;

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
        catch (const Error& error) {
            print(error);
            actual_error = error.what();
        }

        assert(actual_error.empty());
        test_context_cleaned_up();
    }

    static auto test_context_invalid_version() -> void
    {
        std::string actual_error;

        try {
            const Context context {invalid_version};
            static_cast<void>(context);
        }
        catch (const Error& error) {
            print(error);
            actual_error = error.what();
        }

        assert(actual_error == expected_error_invalid_version);
        test_context_cleaned_up();
    }

    static auto test_context_local_instances() -> void
    {
        std::string actual_error;

        try {
            Context context_1 {version_1_0};
            test_context(context_1, "local 1", version_1_0);
            assert(context_1.version() == version_1_0);
            assert(std::string {context_1.version()} == "1.0");
            Context context_2 {version_2_0};
            test_context(context_2, "local 2", version_2_0);
            assert(context_2.version() == version_2_0);
            assert(std::string {context_2.version()} == "2.0");
        }
        catch (const Error& error) {
            print(error);
            actual_error = error.what();
        }

        assert(actual_error.empty());
        test_context_cleaned_up();
    }

    static auto test_context_valid_with_shutdown() -> void
    {
        std::string actual_error;

        try {
            Context context;
            test_context(context, "local 3");
            context.shutdown();
        }
        catch (const Error& error) {
            print(error);
            actual_error = error.what();
        }

        assert(actual_error.empty());
        test_context_cleaned_up();
    }

    static auto test_context_valid_without_shutdown() -> void
    {
        std::string actual_error;

        try {
            const Context context;
            test_context(context, "local 4");
        }
        catch (const Error& error) {
            print(error);
            actual_error = error.what();
        }

        assert(actual_error.empty());
        test_context_cleaned_up();
    }

    static auto test_hostname_running() -> void
    {
        std::string actual_error;

        try {
            const Context context;
            test_context(context, "local 5");
            static_cast<void>(get_hostname(verbose));
        }
        catch (const Error& error) {
            print(error);
            actual_error = error.what();
        }

        assert(actual_error.empty());
        test_context_cleaned_up();
    }

    static auto test_hostname_stopped() -> void
    {
        std::string actual_error;

        try {
            static_cast<void>(get_hostname(verbose));
        }
        catch (const Error& error) {
            print(error);
            actual_error = error.what();
        }

        assert(actual_error == expected_error_stopped);
        test_context_cleaned_up();
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace TestContext;

    try {
        parse_arguments(argc, argv);
        test_context_global_instance();
        test_context_invalid_version();
        test_context_local_instances();
        test_context_valid_with_shutdown();
        test_context_valid_without_shutdown();
        test_hostname_running();
        test_hostname_stopped();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
