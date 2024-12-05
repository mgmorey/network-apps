// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_SOCKET_H
#define NETWORK_SOCKET_H

#include "network/bytestring.h"                 // ByteString
#include "network/family-type.h"                // family_type
#include "network/handle-type.h"                // handle_type
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/readresult.h"                 // ReadResult
#include "network/to-bytestring.h"              // to_bytestring()
#include "network/validate.h"                   // validate()

#include <ostream>      // std::ostream

namespace Network
{
    class Socket
    {
    public:
        Socket() noexcept = default;
        Socket(const Socket&) noexcept = delete;
        Socket(const Socket&&) noexcept = delete;
        virtual ~Socket() noexcept = default;
        auto operator=(const Socket&) noexcept -> Socket& = delete;
        auto operator=(Socket&&) noexcept -> Socket& = delete;
        explicit virtual operator bool() const noexcept = 0;
        explicit virtual operator handle_type() const noexcept = 0;

        [[nodiscard]] virtual auto family() const noexcept -> family_type = 0;
        [[nodiscard]] virtual auto handle() const noexcept -> handle_type = 0;
        [[nodiscard]] virtual auto is_verbose() const noexcept -> bool = 0;

        [[nodiscard]] virtual auto close() -> OsErrorResult = 0;
        [[nodiscard]] virtual auto listen(int t_backlog) const ->
            OsErrorResult = 0;
        [[nodiscard]] virtual auto name(bool t_is_peer) const -> ByteString = 0;
        [[nodiscard]] virtual auto open(const ByteString& t_addr,
                                        bool t_is_bind) -> OsErrorResult = 0;
        [[nodiscard]] virtual auto read(char* t_data,
                                        std::size_t t_size) const -> ssize_t = 0;
        [[nodiscard]] virtual auto read(std::size_t t_size) const -> ReadResult = 0;
        [[nodiscard]] virtual auto write(const char* t_data,
                                         std::size_t t_size) const -> ssize_t = 0;
        [[nodiscard]] virtual auto write(std::string_view t_sv) const -> ssize_t = 0;

        [[nodiscard]] auto bind(const auto& value) -> OsErrorResult
        {
            return open(to_bytestring(validate(value)), true);  // NOLINT
        }

        [[nodiscard]] auto connect(const auto& value) -> OsErrorResult
        {
            return open(to_bytestring(validate(value)), false);
        }

        [[nodiscard]] auto peername() const -> ByteString
        {
            return name(true);
        }

        [[nodiscard]] auto sockname() const -> ByteString
        {
            return name(false);
        }
    };

    extern auto operator<<(std::ostream& os,
                           const Socket& sock) -> std::ostream&;
}

#endif
