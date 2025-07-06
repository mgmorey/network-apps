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

#ifndef NETWORK_INSERT_HOSTNAME_HPP
#define NETWORK_INSERT_HOSTNAME_HPP

#include "network/addresslist.hpp"      // AddressList
#include "network/hostnameview.hpp"     // HostnameView
#include "network/optionalhints.hpp"    // OptionalHints
#include "network/oserror.hpp"          // OsError
#include "network/run.hpp"              // run()
#include "network/serviceview.hpp"      // ServiceView
#include "network/sharedruntime.hpp"    // SharedRuntime

#include <algorithm>    // std::copy()

namespace Network
{
    auto insert(auto it,
                const HostnameView& hostname,
                const ServiceView& service,
                const OptionalHints& hints,
                const SharedRuntime& sr) -> OsError
    {
        const auto list {AddressList(hostname, service, hints, sr)};
        std::copy(list.begin(), list.end(), it);
        return list.result();
    }

    auto insert(auto it,
                const HostnameView& hostname,
                const ServiceView& service,
                const OptionalHints& hints,
                bool is_verbose) -> OsError
    {
        return insert(it, hostname, service, hints, run(is_verbose));
    }
}

#endif
