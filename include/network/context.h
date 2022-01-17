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
        static auto instance() -> Context&;
        explicit Context(const Version& t_version = {});
        Context(const Context&) = delete;
        Context(const Context&&) = delete;
        ~Context();
        auto operator=(const Context&) -> Context& = delete;
        auto operator=(const Context&&) -> Context& = delete;
        [[nodiscard]] auto error() const -> os_error_type;
        [[nodiscard]] auto status() const -> std::string;
        [[nodiscard]] auto system() const -> std::string;
        [[nodiscard]] auto version() const -> Version;

    protected:
        static auto cleanup(bool verbose = false) -> os_error_type;
        auto destroy(bool verbose = false) -> os_error_type;
        auto status(const std::string& t_status) -> void;
        auto system(const std::string& t_system) -> void;
        auto version(const Version& t_version) -> void;

    private:
        os_error_type m_error_code {0};
        bool m_is_started {false};
        std::string m_status;
        std::string m_system;
        Version m_version;
    };

    extern auto operator<<(std::ostream& os,
                           const Context& context) -> std::ostream&;
}

#endif
