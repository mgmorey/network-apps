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

#ifndef NETWORK_OPENINPUTS_HPP
#define NETWORK_OPENINPUTS_HPP

#include "network/endpointview.hpp"     // Endpoint
#include "network/sharedruntime.hpp"    // SharedRuntime
#include "network/sockethints.hpp"      // SocketHints

namespace Network
{
    struct OpenInputs
    {
        OpenInputs(const EndpointView& t_endpoint,
                   const SocketHints& t_hints,
                   const SharedRuntime& t_sr) :
                m_endpoint(t_endpoint), m_hints(t_hints), m_sr(t_sr)
        {
        }

        const EndpointView& m_endpoint;  // NOLINT
        const SocketHints& m_hints;  // NOLINT
        const SharedRuntime& m_sr;  // NOLINT
    };
}

#endif
