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

#include "network/optionalversion.h"    // OptionalVersion

#include <cstdint>      // std::uint8_t
#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    class Context
    {
        friend auto operator<<(std::ostream& os,
                               const Context& context) noexcept ->
            std::ostream&;
        friend auto is_running(const Context& context) -> bool;
        friend auto startup(Context& context,
                            const OptionalVersion& version) -> void;

    public:
        enum class failure_mode : std::uint8_t {
            return_error,
            return_zero,
            throw_error
        };
        static auto instance() -> const Context&;
        explicit Context(const OptionalVersion& t_version = {});
        Context(const Context&) = delete;
        Context(const Context&&) = delete;
        ~Context();
        auto operator=(const Context&) -> Context& = delete;
        auto operator=(const Context&&) -> Context& = delete;

    protected:
        auto shutdown(failure_mode t_mode = failure_mode::throw_error) -> void;

    private:
        std::string m_description;
        std::string m_system_status;
        OptionalVersion m_version;
        bool m_is_started {false};
    };

    extern auto operator<<(std::ostream& os,
                           const Context& context) noexcept -> std::ostream&;
}

#endif
