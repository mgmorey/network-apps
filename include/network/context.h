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

#include "network/optionalversion.h"    // OptionalVersion, Version,
                                        // std::string

#include <ostream>      // std::ostream

namespace Network
{
    class Context
    {
    public:
        enum class failure_mode {return_error, return_zero, throw_error};
        static auto instance() -> const Context&;
        explicit Context(const OptionalVersion& t_version = {});
        Context(const Context&) = delete;
        Context(const Context&&) = delete;
        ~Context();
        auto operator=(const Context&) -> Context& = delete;
        auto operator=(const Context&&) -> Context& = delete;
        auto description(const std::string& t_description) -> Context&;
        auto is_started(bool t_is_started) noexcept -> Context&;
        auto system_status(const std::string& t_status) -> Context&;
        auto version(const Version& t_version) noexcept -> Context&;
        [[nodiscard]] auto description() const -> const std::string&;
        [[nodiscard]] auto is_started() const noexcept -> bool;
        [[nodiscard]] auto system_status() const -> const std::string&;
        [[nodiscard]] auto version() const noexcept -> Version;

    protected:
        auto shutdown(failure_mode t_mode = failure_mode::throw_error) -> void;

    private:
        bool m_is_started {false};
        std::string m_description;
        std::string m_system_status;
        Version m_version;
    };

    extern auto operator<<(std::ostream& os,
                           const Context& context) -> std::ostream&;
}

#endif
