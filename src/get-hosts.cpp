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

#include "network/get-hosts.h"      // Hints, Host, Hostname,
                                    // HostVector, HostVectorResult,
                                    // Result, get_hosts()
#include "network/addrinfo.h"       // AddrInfo
#include "network/get-hostname.h"   // HostnameResult, get_hostname()
#include "network/overloaded.h"     // Overloaded
#include "network/uniquify.h"       // Uniquify

#include <iterator>     // std::back_inserter()
#include <optional>     // std::nullopt
#include <string>       // std::string
#include <variant>      // std::visit()

auto Network::get_hosts(const Network::Hostname& host,
                        const Network::Hints* hints,
                        bool verbose) -> Network::HostVectorResult
{
    HostVectorResult hosts_result;
    const auto hostname_result {get_hostname(host)};
    std::visit(Overloaded {
            [&](const std::string& hostname) {
                HostVector hosts;
                const auto result {
                    AddrInfo::insert(hostname,
                                     std::nullopt,
                                     hints,
                                     verbose,
                                     std::back_inserter(hosts))
                };

                if (result) {
                    hosts_result = result;
                }
                else {
                    uniquify(hosts);
                    hosts_result = hosts;
                }
            },
            [&](const Result& result) {
                hosts_result = result;
            }
        }, hostname_result);
    return hosts_result;
}
