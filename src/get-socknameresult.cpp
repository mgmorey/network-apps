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

#include "network/get-socknameresult.h" // get_socknameresult()
#include "network/get-nameresult.h"     // get_nameresult()
#include "network/getnameparams.h"      // GetNameParams
#include "network/handle-type.h"        // handle_type
#include "network/socknameresult.h"     // SockNameResult

auto Network::get_socknameresult(handle_type handle, bool is_verbose) ->
    SockNameResult
{
    const GetNameParams args {.handle = handle, .is_verbose = is_verbose};
    return get_nameresult(args, false);
}
