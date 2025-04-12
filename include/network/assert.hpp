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

#if defined(NDEBUG)
#undef assert
#define assert(e) (static_cast<void>(0))  // NOLINT
#elif defined(OS_DARWIN)
#undef assert
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
#undef assert
// NOLINTNEXTLINE
#define assert(e) ((e) ? static_cast<void>(0) :                         \
                   __assert(static_cast<const char*>(__func__),         \
                            __FILE__,                                   \
                            __LINE__,                                   \
                            #e))
#elif defined(OS_LINUX)
#undef assert
/* When possible, define assert so that it does not add extra
   parentheses around EXPR.  Otherwise, those added parentheses would
   suppress warnings we'd expect to be detected by gcc's
   -Wparentheses.  */
# if defined __cplusplus
// NOLINTNEXTLINE
#  define assert(expr)							\
     (static_cast <bool> (expr)						\
      ? void (0)							\
      : __assert_fail (#expr,                                           \
                       __FILE__,                                        \
                       __LINE__,                                        \
                       static_cast<const char*>(__ASSERT_FUNCTION)))
# elif !defined __GNUC__ || defined __STRICT_ANSI__
#  define assert(expr)							\
    ((expr)								\
     ? __ASSERT_VOID_CAST (0)						\
     : __assert_fail (#expr, __FILE__, __LINE__, __ASSERT_FUNCTION))
# else
/* The first occurrence of EXPR is not evaluated due to the sizeof,
   but will trigger any pedantic warnings masked by the __extension__
   for the second occurrence.  The ternary operator is required to
   support function pointers and bit fields in this context, and to
   suppress the evaluation of variable length arrays.  */
#  define assert(expr)							\
  ((void) sizeof ((expr) ? 1 : 0), __extension__ ({			\
      if (expr)								\
        ; /* empty */							\
      else								\
        __assert_fail (#expr, __FILE__, __LINE__, __ASSERT_FUNCTION);	\
    }))
# endif
#endif

#endif
