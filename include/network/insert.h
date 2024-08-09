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

#ifndef NETWORK_INSERT_H
#define NETWORK_INSERT_H

#include "network/addresslist.h"        // AddressList
#include "network/optionalhints.h"      // OptionalHints
#include "network/optionalhostname.h"   // OptionalHostname
#include "network/optionalservice.h"    // OptionalService
#include "network/oserrorresult.h"      // OsErrorResult

#include <iostream>     // std::cerr, std::endl
#include <iterator>     // std::back_inserter()

namespace Network
{
    auto insert(const OptionalHostname& hostname,
                const OptionalService& service,
                const OptionalHints& hints,
                auto& container,
                bool verbose) -> OsErrorResult
    {
        const auto list {AddressList(hostname, service, hints, verbose)};
        auto it {std::back_inserter(container)};

        for (const auto& item : list) {
            if (verbose) {
                std::cerr << item
                          << std::endl;
            }

            *it++ = item;
        }

        return list.result();
    }
}

#endif
