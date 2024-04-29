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

#include "network/remove-socket.h"              // remove_socket()
#include "network/optionalpathname.h"           // OptionalPathanme

#include <filesystem>   // std::filesystem
#include <iostream>     // std::cout, std::endl

auto Network::remove(const Pathname& path, bool verbose) -> bool
{
    if (verbose) {
        std::cout << "Calling std::filesystem::remove"
                  << '('
                  << path
                  << ')'
                  << std::endl;
    }

    return std::filesystem::remove(path);
}

#endif
