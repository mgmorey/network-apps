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
#undef assert

#if defined(NDEBUG)
#define assert(e) (static_cast<void>(0))  // NOLINT
#elif defined(OS_DARWIN)
#ifndef __GNUC__
#define assert(e)                                                       \
    (static_cast<void>(((e) ?                                           \
                        (static_cast<void>(0)) :                        \
                        __assert (#e, __ASSERT_FILE_NAME, __LINE__))))
#else // __GNUC__
#ifdef __DARWIN_UNIX03
// NOLINTNEXTLINE
#define	assert(e) \
    (__builtin_expect(!(e), 0) ?                      \
     __assert_rtn(static_cast<const char*>(__func__), \
                  __ASSERT_FILE_NAME, __LINE__, #e) : \
     static_cast<void>(0))
#else // !__DARWIN_UNIX03
// NOLINTNEXTLINE
#define assert(e) \
    (__builtin_expect(!(e), 0) ?                   \
     __assert (#e, __ASSERT_FILE_NAME, __LINE__) : \
     static_cast<void>(0))
#endif // __DARWIN_UNIX03
#endif // __GNUC__
#elif defined(OS_FREEBSD)
// NOLINTNEXTLINE
#define assert(e) ((e) ? static_cast<void>(0) :                         \
                   __assert(static_cast<const char*>(__func__),         \
                            __FILE__,                                   \
                            __LINE__,                                   \
                            #e))
#endif

#endif
