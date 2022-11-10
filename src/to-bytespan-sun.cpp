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

#include "network/to-bytespan-sun.h"    // ByteSpan, sockaddr_un,
                                        // std::size_t, to_bytespan()
#include "network/to-bytespan-void.h"   // to_bytespan()

#ifndef WIN32

auto Network::to_bytespan(const sockaddr_un* sun,
                          std::size_t size) noexcept -> Network::ByteSpan
{
    const void* pointer = sun;
    return to_bytespan(pointer, size);
}

#endif
