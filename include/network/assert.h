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

#ifndef NETWORK_ASSERT_H
#define NETWORK_ASSERT_H

#include <cassert>

#ifdef USING_CUSTOM_ASSERT
#undef  assert
#ifdef NDEBUG
// NOLINTNEXTLINE
#define assert(e)   (static_cast<void>(0))
#else
// NOLINTNEXTLINE
#define assert(e)   ((e) ? static_cast<void>(0) :                       \
                         __assert(static_cast<const char*>(__func__),   \
                                  __FILE__,                             \
                                  __LINE__,                             \
                                  #e))
#endif // NDEBUG
#endif // USING_CUSTOM_ASSERT

#endif
