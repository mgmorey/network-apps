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

#include "network/contextdata.h"        // ContextData
#include "network/errorresult.h"        // ErrorResult
#include "network/version.h"            // Version

#include <optional>     // std::optional
#include <ostream>      // std::ostream

namespace Network
{
    class Context
    {
        friend auto operator<<(std::ostream& os,
                               const Context& context) -> std::ostream&;

    public:
        using OptionalVersion = std::optional<Version>;

        explicit Context(const OptionalVersion& t_version = {});
        Context(const Context&) = delete;
        Context(const Context&&) = delete;
        ~Context();
        auto operator=(const Context&) -> Context& = delete;
        auto operator=(const Context&&) -> Context& = delete;
        [[nodiscard]] auto status() const -> std::string;
        [[nodiscard]] auto system() const -> std::string;
        [[nodiscard]] auto version() const -> Version;

    protected:
        static auto cleanup(error_type error_code = 0) -> error_type;
        static auto dispatch(error_type error_code) -> void;

    private:
        ContextData m_data;
        error_type m_error_code {0};
    };

    extern auto operator<<(std::ostream& os,
                           const Context& context) -> std::ostream&;
}

#endif
