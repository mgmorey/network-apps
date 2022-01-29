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

#include "network/os-error-type.h"      // os_error_type
#include "network/version.h"            // Version, version_null

#include <ostream>      // std::ostream

namespace Network
{
    class Context
    {
    public:
        static auto instance() -> const Context&;
        explicit Context(const Version& t_version = {});
        Context(const Context&) = delete;
        Context(const Context&&) = delete;
        ~Context() noexcept;
        auto operator=(const Context&) -> Context& = delete;
        auto operator=(const Context&&) -> Context& = delete;
        [[nodiscard]] auto description() const noexcept -> std::string;
        [[nodiscard]] auto is_started() const noexcept -> bool;
        [[nodiscard]] auto system_status() const noexcept -> std::string;
        [[nodiscard]] auto version() const noexcept -> Version;

    protected:
        enum class failure_mode {return_error, return_zero, throw_error};
        static auto cleanup(failure_mode t_mode =
                            failure_mode::throw_error) -> os_error_type;
        auto is_started(bool t_is_started) noexcept -> void;
        auto shutdown(failure_mode t_mode = failure_mode::throw_error) -> void;

    private:
        bool m_is_started {false};
        std::string m_description;
        std::string m_system_status;
        Version m_version;
    };

    extern auto operator<<(std::ostream& os,
                           const Context& context) noexcept -> std::ostream&;
}

#endif
