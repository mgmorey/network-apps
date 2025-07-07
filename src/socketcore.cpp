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

#include "network/socketcore.hpp"       // SocketCore
#include "network/family-null.hpp"      // family_null
#include "network/family-type.hpp"      // family_type
#include "network/handle-null.hpp"      // handle_null
#include "network/handle-type.hpp"      // handle_type
#include "network/logicerror.hpp"       // LogicError
#include "network/sharedruntime.hpp"    // SharedRuntime

#include <string_view>  // std::string_view

Network::SocketCore::SocketCore(handle_type t_handle,
                                family_type t_family,
                                const SharedRuntime& t_sr) :
    m_runtime(t_sr),
    m_handle(t_handle),
    m_family(t_family)
{
    std::string_view error;

    if (m_runtime == nullptr) {
        error = "Null runtime pointer";
    }
    else if (m_handle == handle_null) {
        error = "Null socket descriptor";
    }
    else if (m_family == family_null) {
        error = "Null socket domain/family";
    }

    if (!error.empty()) {
        throw LogicError(error);
    }
}

auto Network::SocketCore::family() const noexcept -> family_type
{
    return m_family;
}

auto Network::SocketCore::handle() const noexcept -> handle_type
{
    return m_handle;
}

auto Network::SocketCore::runtime() const noexcept -> SharedRuntime
{
    return m_runtime;
}
