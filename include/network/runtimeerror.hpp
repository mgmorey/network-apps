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

#ifndef NETWORK_RUNTIMEERROR_HPP
#define NETWORK_RUNTIMEERROR_HPP

#include "network/error.hpp"            // Error

#include <string_view>  // std::string_view

namespace Network
{
    class RuntimeError : public Error
    {
    public:
        explicit RuntimeError(std::string_view t_sv) noexcept;
        RuntimeError(const RuntimeError&) noexcept = default;
        RuntimeError(RuntimeError&&) noexcept = default;
        ~RuntimeError() noexcept override = default;
        auto operator=(const RuntimeError&) noexcept -> RuntimeError& = default;
        auto operator=(RuntimeError&&) noexcept -> RuntimeError& = default;
    };
}

#endif
