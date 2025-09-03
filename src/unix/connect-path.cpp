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

#ifndef _WIN32

#include "network/connect-path.hpp"             // connect()
#include "network/create-socket-hints.hpp"      // create_socket()
#include "network/pathnameview.hpp"             // PathnameView
#include "network/run.hpp"                      // run()
#include "network/sharedruntime.hpp"            // SharedRuntime
#include "network/sockethints.hpp"              // SocketHints
#include "network/socketresult.hpp"             // SocketResult

auto Network::connect(const PathnameView& pathname,
                      const SocketHints& hints,
                      const SharedRuntime& sr) -> SocketResult
{
    auto s {create_socket(hints, sr->is_verbose())};

    if (auto error {s->connect(pathname)}) {
        return std::unexpected(error);
    }

    return s;
}

auto Network::connect(const PathnameView& pathname,
                      const SocketHints& hints,
                      bool is_verbose) -> SocketResult
{
    return connect(pathname, hints, run(is_verbose));
}

#endif
