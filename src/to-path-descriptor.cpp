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

#ifndef WIN32

#include "network/to-path-descriptor.h"         // to_path()
#include "network/descriptor-type.h"            // descriptor_type
#include "network/get-sockname.h"               // get_sockname()
#include "network/optionalpathname.h"           // OptionalPathname
#include "network/to-path-bytestring.h"         // to_path()

auto Network::to_path(descriptor_type handle, bool verbose) -> OptionalPathname
{
    return to_path(get_sockname(handle, verbose));
}

#endif
