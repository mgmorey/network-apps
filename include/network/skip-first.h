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

#ifndef NETWORK_SKIP_FIRST_H
#define NETWORK_SKIP_FIRST_H

#include "network/iterator-pair.h"      // iterator_pair

#include <iterator>     // std::next()

namespace Network
{
    template <typename Range>
    auto skip_first(Range& r) -> iterator_pair<decltype(r.begin())>
    {
        return {r.begin() == r.end() ? r.begin() : std::next(r.begin()), r.end()};
    }
}

#endif
