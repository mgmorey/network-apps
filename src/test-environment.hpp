// Copyright (C) 2026  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_TEST_ENVIRONMENT_HPP
#define NETWORK_TEST_ENVIRONMENT_HPP

#include "network/error.hpp"          // Error

#include <cstdlib>      // std::free(), std::getenv()
#include <memory>       // std::unique_ptr
#include <string>       // std::to_string()

namespace
{
    auto test_has_environment_variable(const char* name) -> bool
    {
#ifdef _MSC_VER
        char* value {};
        std::size_t size {};
        const auto error {::_dupenv_s(&value, &size, name)};
        const std::unique_ptr<char, decltype(&std::free)> result {
            value, &std::free
        };
        if (error != 0) {
            throw Network::Error {
                "Call to _dupenv_s failed with error "
                + std::to_string(error)
            };
        }
        return result != nullptr;
#else
        return std::getenv(name) != nullptr;
#endif
    }
}

#endif
