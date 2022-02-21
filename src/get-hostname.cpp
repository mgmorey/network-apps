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

#include "network/get-hostname.h"       // Hostname,
                                        // get_hostname()
#include "network/error.h"              // Error
#include "network/get-hostnameresult.h" // OsErrorResult,
                                        // get_hostname()
#include "network/overloaded.h"         // Overloaded

#include <variant>      // std::visit()

auto Network::get_hostname() -> Network::Hostname
{
    Hostname result;
    const auto hostname_result {get_hostnameresult()};
    std::visit(Overloaded {
            [&](const Hostname& hostname) {
                result = hostname;
            },
            [&](const OsErrorResult& error) {
                throw Error(error.string());
            }
        }, hostname_result);
    return result;
}
