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

#ifndef NETWORK_INSERT_HOSTNAME_H
#define NETWORK_INSERT_HOSTNAME_H

#include "network/addresslist.h"        // AddressList
#include "network/hostnameview.h"       // HostnameView
#include "network/optionalhints.h"      // OptionalHints
#include "network/oserrorresult.h"      // OsErrorResult
#include "network/serviceview.h"        // ServiceView

#include <algorithm>    // std::copy()
#include <iterator>     // std::back_inserter()

namespace Network
{
    auto insert(auto& container,
                const HostnameView& hostname,
                const ServiceView& service = {},
                const OptionalHints& hints = {},
                bool is_verbose = false) -> OsErrorResult
    {
        const auto list {AddressList(hostname, service, hints, is_verbose)};
        auto it {std::back_inserter(container)};
        std::copy(list.begin(), list.end(), it);
        return list.result();
    }
}

#endif
