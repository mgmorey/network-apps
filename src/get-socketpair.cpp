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

#include "network/get-socketpair.h"             // FdPair,
                                                // SocketHints,
                                                // get_socketpair()
#include "network/error.h"                      // Error
#include "network/get-socketpairresult.h"       // FdPairResult,
                                                // get_socketpairresult()

#include "network/overloaded.h"         // Overloaded

#include <optional>     // std::optional
#include <variant>      // std::visit()

#ifndef WIN32

auto Network::get_socketpair(const SocketHints& hints,
                             bool verbose) -> Network::FdPair
{
    std::string result_string;
    std::optional<FdPair> result_fds;
    const auto socketpair_result {get_socketpairresult(hints, verbose)};
    std::visit(Overloaded {
            [&](const FdPair& fds) {
                result_fds = fds;
            },
            [&](const OsErrorResult& result) {
                result_string = result.string();
            }
        }, socketpair_result);

    if (result_fds) {
        return *result_fds;
    }

    throw Error(result_string);
}

#endif
