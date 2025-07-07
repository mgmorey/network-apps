// Copyright (C) 2025  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_ACCEPTDATA_HPP
#define NETWORK_ACCEPTDATA_HPP

#include "network/bytestring.hpp"               // ByteString
#include "network/family-type.hpp"              // family_type
#include "network/long-handle-type.hpp"         // long_handle_type
#include "network/sharedruntime.hpp"            // SharedRuntime

namespace Network
{
    struct AcceptData
    {
        AcceptData(long_handle_type t_handle, family_type t_family,
                   const SharedRuntime& t_runtime,
                   const ByteString& t_accept) :
            m_runtime(t_runtime), m_accept(t_accept),
            m_handle(t_handle), m_family(t_family)
        {
        }

        [[nodiscard]] auto accept() const noexcept -> ByteString
        {
            return m_accept;
        }

        [[nodiscard]] auto family() const noexcept -> family_type
        {
            return m_family;
        }

        [[nodiscard]] auto handle() const noexcept -> long_handle_type
        {
            return m_handle;
        }

        [[nodiscard]] auto runtime() const noexcept -> SharedRuntime
        {
            return m_runtime;
        }

    private:
        SharedRuntime m_runtime;
        ByteString m_accept;
        long_handle_type m_handle;
        family_type m_family;
    };
}

#endif
