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

#ifndef NETWORK_FDDATA_H
#define NETWORK_FDDATA_H

#include "network/fd-null.h"                    // fd_null
#include "network/fd-type.h"                    // fd_type

#include <string>      // std::string

namespace Network
{
    class FdData
    {
    public:
        explicit FdData(fd_type t_fd_data,
                        bool t_removal,
                        bool t_verbose = false) noexcept;
        FdData(const FdData&) noexcept = default;
        FdData(FdData&&) noexcept = default;
        FdData() noexcept = default;
        ~FdData();
        auto operator=(const FdData&) noexcept -> FdData& = default;
        auto operator=(FdData&&) noexcept -> FdData& = default;
        auto operator=(fd_type value) noexcept -> FdData&;
        explicit operator bool() const noexcept;
        explicit operator fd_type() const noexcept;
        explicit operator std::string() const;
        auto close() -> FdData&;
        [[nodiscard]] auto handle() const noexcept -> fd_type;
        [[nodiscard]] auto removal() const noexcept -> bool;
        [[nodiscard]] auto verbose() const noexcept -> bool;
        auto removal(bool value) noexcept -> FdData&;
        auto verbose(bool value) noexcept -> FdData&;

    private:
        fd_type m_fd_data {fd_null};
        bool m_removal {false};
        bool m_verbose {false};
    };
}

#endif
