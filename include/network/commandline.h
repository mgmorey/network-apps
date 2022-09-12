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

#ifndef NETWORK_COMMANDLINE_H
#define NETWORK_COMMANDLINE_H

#include "network/optionalstring.h"     // OptionalString

#include <span>         // std::span
#include <string>       // std::string
#include <vector>       // std::vector

namespace Network
{
    class CommandLine
    {
    public:
        using Argument = std::string;
        using Arguments = std::vector<Argument>;

        CommandLine(int t_argc,
                    char** t_argv,
                    const OptionalString& t_options = {});
        CommandLine(const CommandLine&) noexcept = default;
        CommandLine(CommandLine&&) noexcept = default;
        ~CommandLine() noexcept = default;
        auto operator=(const CommandLine&) noexcept -> CommandLine& = default;
        auto operator=(CommandLine&&) noexcept -> CommandLine& = default;
        [[nodiscard]] auto arguments() const -> Arguments;
        [[nodiscard]] auto option() const -> int;

    private:
        std::span<char*> m_args;
        OptionalString m_opts;
    };
}

#endif
