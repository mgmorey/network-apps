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

#include "network/get-hosts.h"          // ErrorResult, Hostname,
                                        // HostVector,
                                        // HostVectorResult,
                                        // OptionalHints, get_hosts()
#include "network/addrinfo.h"           // AddrInfo
#include "network/overloaded.h"         // Overloaded
#include "network/uniquify.h"           // uniquify()

#include <algorithm>    // std::sort()
#include <iterator>     // std::back_inserter()
#include <optional>     // std::nullopt

auto Network::get_hosts(const Network::Hostname& hostname,
                        const Network::OptionalHints& hints,
                        bool verbose) -> Network::HostVectorResult
{
    HostVector hosts;
    const auto result {
        AddrInfo::insert(static_cast<OptionalHostname>(hostname),
                         static_cast<OptionalService>(std::nullopt),
                         hints,
                         verbose,
                         std::back_inserter(hosts))
    };

    if (result) {
        return result;
    }

    std::sort(hosts.begin(), hosts.end());
    uniquify(hosts);
    return hosts;
}
