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

#ifdef NDEBUG

// NOLINTNEXTLINE
#define assert(e)   (static_cast<void>(0))

#else // !NDEBUG

#if defined(OS_DARWIN)

#undef assert
#undef __assert

#ifndef __GNUC__

#define assert(e)                                                       \
    ((void) ((e) ? ((void)0) : __assert (#e, __ASSERT_FILE_NAME, __LINE__)))
#define __assert(e, file, line) \
    ((void)printf ("%s:%d: failed assertion `%s'\n", file, line, e), abort())

#else // __GNUC__

#if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && ((__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__-0) < 1070)
// NOLINTNEXTLINE
#define __assert(e, file, line)                                 \
    __eprintf ("%s:%d: failed assertion `%s'\n", file, line, e)
#else
/* 8462256: modified __assert_rtn() replaces deprecated __eprintf() */
// NOLINTNEXTLINE
#define __assert(e, file, line)                         \
    __assert_rtn ((const char *)-1L, file, line, e)
#endif

#if __DARWIN_UNIX03
// NOLINTNEXTLINE
#define	assert(e) \
    (__builtin_expect(!(e), 0) ?                      \
     __assert_rtn(static_cast<const char*>(__func__), \
                  __ASSERT_FILE_NAME, __LINE__, #e) : \
     static_cast<void>(0))
#else // !__DARWIN_UNIX03
// NOLINTNEXTLINE
#define assert(e)  \
    (__builtin_expect(!(e), 0) ?                   \
     __assert (#e, __ASSERT_FILE_NAME, __LINE__) : \
     (void)0)
#endif // __DARWIN_UNIX03

#endif // __GNUC__

#elif defined(OS_FREEBSD)

#undef assert

// NOLINTNEXTLINE
#define assert(e)   ((e) ? static_cast<void>(0) :                       \
                         __assert(static_cast<const char*>(__func__),   \
                                  __FILE__,                             \
                                  __LINE__,                             \
                                  #e))

#endif

#endif // NDEBUG

#endif
