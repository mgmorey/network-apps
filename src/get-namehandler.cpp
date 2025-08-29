// Copyright (C) 2025  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/get-namehandler.hpp"  // get_namehandler()
#include "network/error-strings.hpp"    // HANDLER_ERROR
#include "network/logicerror.hpp"       // LogicError
#include "network/namehandler.hpp"      // NameHandler
#include "network/namesymbol.hpp"       // NameSymbol

#ifdef _WIN32
#include <winsock2.h>       // ::getpeername(), ::getsockname()
#else
#include <sys/socket.h>     // ::getpeername(), ::getsockname()
#endif

auto Network::get_namehandler(NameSymbol symbol) -> NameHandler
{
    switch (symbol) {
    case NameSymbol::getsockname:
        return {::getsockname, "::getsockname"};
    case NameSymbol::getpeername:
        return {::getpeername, "::getpeername"};
    default:
        throw LogicError {HANDLER_ERROR};
    }
}
