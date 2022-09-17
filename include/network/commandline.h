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

#include <cstddef>      // std::size_t
#include <span>         // std::span

namespace Network
{
    class CommandLine
    {
    public:
        using Argument = char*;
        using ArgumentSpan = std::span<Argument>;

        CommandLine(int t_argc, char** t_argv);
        CommandLine(const CommandLine&) noexcept = default;
        CommandLine(CommandLine&&) noexcept = default;
        ~CommandLine() noexcept = default;
        auto operator=(const CommandLine&) noexcept -> CommandLine& = default;
        auto operator=(CommandLine&&) noexcept -> CommandLine& = default;
        [[nodiscard]] auto operator[](std::size_t t_offset) const -> Argument;
        [[nodiscard]] auto operator[](int t_offset) const -> Argument;
        [[nodiscard]] auto option(const char* t_optstring) const -> int;
        [[nodiscard]] auto span(std::size_t t_offset,
                                std::size_t t_count = std::dynamic_extent) ->
            ArgumentSpan;
        [[nodiscard]] auto span(int t_offset = -1,
                                std::size_t t_count = std::dynamic_extent) ->
            ArgumentSpan;

    private:
        char** m_data {nullptr};
        std::size_t m_size {};
        ArgumentSpan m_span;
    };
}

#endif
