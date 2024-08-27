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

#ifndef NETWORK_CONTEXT_H
#define NETWORK_CONTEXT_H

#include "network/context-error-type.h"     // context_error_type
#include "network/optionalversion.h"        // OptionalVersion

#include <cstdint>      // std::uint8_t
#include <ostream>      // std::ostream
#include <string>       // std::string
#include <string_view>  // std::string_view

namespace Network
{
    class Context
    {
        friend auto operator<<(std::ostream& os,
                               const Context& context) -> std::ostream&;
        friend auto is_running(const Context& context) -> bool;

    public:
        enum class failure_mode : std::uint8_t {
            return_error,
            return_zero,
            throw_error
        };

        static auto instance() -> const Context&;

        explicit Context(const OptionalVersion& t_version = {},
                         bool t_is_verbose = false);
        Context(const Context&) = delete;
        Context(const Context&&) = delete;
        virtual ~Context();
        auto operator=(const Context&) -> Context& = delete;
        auto operator=(const Context&&) -> Context& = delete;
        [[nodiscard]] auto error_code() const noexcept -> context_error_type;
        [[nodiscard]] auto is_started() const noexcept -> bool;
        [[nodiscard]] auto is_verbose() const noexcept -> bool;
        auto set(const std::string_view& t_description,
                 const std::string_view& t_system_status,
                 OptionalVersion t_version) -> Context&;
        auto start(const OptionalVersion& t_version) -> Context&;
        auto stop(failure_mode mode) -> Context&;

    private:
        std::string m_description;
        std::string m_system_status;
        OptionalVersion m_version;
        context_error_type m_error_code {0};
        bool m_is_started {false};
        bool m_is_verbose {false};
    };

    extern auto operator<<(std::ostream& os,
                           const Context& context) -> std::ostream&;
}

#endif
