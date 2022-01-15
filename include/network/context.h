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

#include "network/errorresult.h"        // ErrorResult

#ifdef _WIN32
#include <winsock2.h>       // WSADATA
#include <windows.h>        // WORD
#endif

#include <optional>     // std::optional
#include <ostream>      // std::ostream

namespace Network
{
    class Context
    {
        friend auto operator<<(std::ostream& os,
                               const Context& context) -> std::ostream&;

    public:
#ifdef _WIN32
        using version_type = WORD;
#else
        using version_type = unsigned;
#endif
        using OptionalVersion = std::optional<version_type>;

        explicit Context(const OptionalVersion& t_version = {});
        Context(const Context&) = delete;
        Context(const Context&&) = delete;
        ~Context();
        auto operator=(const Context&) -> Context& = delete;
        auto operator=(const Context&&) -> Context& = delete;
        static auto cleanup() -> void;
        [[nodiscard]] static auto create(version_type version) -> error_type;
        static auto destroy(error_type error_code) -> void;
        static auto dispatch(error_type error_code) -> void;
        [[nodiscard]] static auto startup(version_type version) -> error_type;
        [[nodiscard]] static auto status_string() -> std::string;
        [[nodiscard]] static auto system_string() -> std::string;
        [[nodiscard]] static auto version_number() -> version_type;
        [[nodiscard]] static auto version_string() -> std::string;

    private:
        static unsigned m_count;
#ifdef _WIN32
        static WSADATA m_wsadata;
#endif
        error_type m_error_code;
    };

    extern auto operator<<(std::ostream& os,
                           const Context& context) -> std::ostream&;
}

#endif
