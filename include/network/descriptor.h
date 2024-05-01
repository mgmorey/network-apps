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

#ifndef NETWORK_DESCRIPTOR_H
#define NETWORK_DESCRIPTOR_H

#include "network/bytestring.h"                 // ByteString
#include "network/descriptor-null.h"            // descriptor_null
#include "network/descriptor-type.h"            // descriptor_type
#include "network/optionalbytestring.h"         // OptionalByteString
#ifndef WIN32
#include "network/optionalpathname.h"           // OptionalPathanme
#endif

namespace Network
{
    class Descriptor
    {
    public:
        explicit Descriptor(descriptor_type t_handle,
                            bool t_verbose = false) noexcept;
        Descriptor(const Descriptor&) noexcept = delete;
        Descriptor(Descriptor&&) noexcept = delete;
        ~Descriptor() noexcept;
        auto operator=(const Descriptor&) noexcept -> Descriptor& = delete;
        auto operator=(Descriptor&&) noexcept -> Descriptor& = delete;
        auto operator=(descriptor_type value) noexcept -> Descriptor&;
        auto bound(bool t_bound) -> Descriptor&;
        auto close() -> Descriptor&;
        [[nodiscard]] auto handle() const noexcept -> descriptor_type;
        [[nodiscard]] auto peername() const -> ByteString;
        [[nodiscard]] auto sockname() const -> ByteString;
        [[nodiscard]] auto verbose() const noexcept -> bool;

    private:
        mutable OptionalByteString m_peername {};
        mutable OptionalByteString m_sockname {};
#ifndef WIN32
        OptionalPathname m_pathname {};
#endif
        descriptor_type m_handle {descriptor_null};
        bool m_verbose {false};
    };
}

#endif
