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

#include "network/assert.hpp"           // assert()
#include "network/version.hpp"          // Version

#include <iostream>     // std::cerr, std::cout, std::endl

namespace
{
    using Network::Version;

    auto test_version() -> void
    {
        constexpr Version v0_0 {0, 0};
        constexpr Version v0_5 {0, 5};
        constexpr Version v1_0 {1, 0};
        constexpr Version v1_5 {1, 5};
        constexpr Version v2_0 {2, 0};
        constexpr Version v0 {Version {0, 0}};
        constexpr Version v1 = v1_0;
        constexpr Version v2 {v2_0};
        static_assert(v0 != v1);
        static_assert(v1 != v2);
        static_assert(v0 == v0_0);
        static_assert(v1 == v1_0);
        static_assert(v2 == v2_0);
        static_assert(v0_0 < v0_5);
        static_assert(v0_5 < v1_0);
        static_assert(v1_0 < v1_5);
        static_assert(v1_5 < v2_0);
        static_assert(v2_0 > v0_0);
        std::cout << v0 << std::endl;
        std::cout << v1 << std::endl;
        std::cout << v2 << std::endl;
    }
}

auto main() -> int
{
    try {
        test_version();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
