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

#include "network/assert.h"             // assert()
#include "network/network.h"            // Version,

#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <iostream>     // std::cerr, std::cout, std::endl

namespace
{
    using Network::OptionalVersion;
    using Network::Version;

    auto test_version() -> void
    {
        const Version v0_0 {0, 0};
        const Version v0_5 {0, 5};
        const Version v1_0 {1, 0};
        const Version v1_5 {1, 5};
        const Version v2_0 {2, 0};
        const Version v0 {Version {0, 0}};
        const Version v1 = v1_0;
        const Version v2 {v2_0};
        assert(v0 != v1);
        assert(v1 != v2);
        assert(v0 == v0_0);
        assert(v1 == v1_0);
        assert(v2 == v2_0);
        assert(v0_0 < v0_5);
        assert(v0_5 < v1_0);
        assert(v1_0 < v1_5);
        assert(v1_5 < v2_0);
        assert(v2_0 > v0_0);
        std::cout << v0 << std::endl;
        std::cout << v1 << std::endl;
        std::cout << v2 << std::endl;
    }
}

auto main(int argc, char* argv[]) -> int
{
    static_cast<void>(argc);
    static_cast<void>(argv);

    try {
        test_version();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
