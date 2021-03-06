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

#ifndef NETWORK_SKIP_LAST_H
#define NETWORK_SKIP_LAST_H

#include "network/iteratorpair.h"       // IteratorPair

#include <iterator>     // std::prev()

namespace Network
{
    template <typename Range>
    auto skip_last(Range& r) -> IteratorPair<decltype(r.begin())>
    {
        return {r.begin(), r.begin() == r.end() ? r.end() : std::prev(r.end())};
    }
}

#endif
