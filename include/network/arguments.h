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

#ifndef NETWORK_ARGUMENTS_H
#define NETWORK_ARGUMENTS_H

#include "network/argument.h"           // Argument
#include "network/argumentspan.h"       // ArgumentSpan

#include <cstddef>      // std::size_t
#include <span>         // std::span

namespace Network
{
    class Arguments
    {
        friend auto get_option(const Arguments& args,
                               const char* optstring) -> int;

    public:
        Arguments(std::size_t t_argc, char** t_argv);
        Arguments(int t_argc, char** t_argv);
        Arguments(const Arguments&) noexcept = default;
        Arguments(Arguments&&) noexcept = default;
        ~Arguments() noexcept = default;
        auto operator=(const Arguments&) noexcept -> Arguments& = default;
        auto operator=(Arguments&&) noexcept -> Arguments& = default;
        [[nodiscard]] auto operator[](std::size_t t_offset) const -> Argument;
        [[nodiscard]] auto operator[](int t_offset) const -> Argument;
        [[nodiscard]] auto arguments() -> ArgumentSpan;
        [[nodiscard]] auto optional() -> ArgumentSpan;
        [[nodiscard]] auto required() -> ArgumentSpan;

    protected:
        [[nodiscard]] auto data() const -> Argument const*;
        [[nodiscard]] auto size() const -> std::size_t;

    private:
        ArgumentSpan m_args;
    };
}

#endif
