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

#include "network/remove-socket.h"              // remove_socket()
#include "network/to-path-descriptor.h"         // to_path()

#include <filesystem>   // std::filesystem
#include <iostream>     // std::cout, std::endl

auto Network::remove_socket(descriptor_type handle, bool verbose) -> bool
{
    if (const auto path {to_path(handle, verbose)}) {
        const bool result {std::filesystem::remove(*path)};

        if (result) {
            if (verbose) {
                std::cout << "Removed "
                          << *path
                          << std::endl;
            }
        }

        return result;
    }

    return false;
}
