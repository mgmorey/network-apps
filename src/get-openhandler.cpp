// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/get-openhandler.hpp"  // get_openhandler()
#include "network/error-strings.hpp"    // HANDLER_ERROR
#include "network/logicerror.hpp"       // LogicError
#include "network/openhandler.hpp"      // OpenHandler
#include "network/opensymbol.hpp"       // OpenSymbol

#ifdef _WIN32
#include <winsock2.h>       // ::bind(), ::connect()
#else
#include <sys/socket.h>     // ::bind(), ::connect()
#endif

auto Network::get_openhandler(OpenSymbol symbol) -> OpenHandler
{
    switch (symbol) {
    case OpenSymbol::bind:
        return {::bind, "::bind"};
    case OpenSymbol::connect:
        return {::connect, "::connect"};
    default:
        throw LogicError {HANDLER_ERROR};
    }
}
