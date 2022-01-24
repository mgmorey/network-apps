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

#ifndef NETWORK_OS_FEATURES_H
#define NETWORK_OS_FEATURES_H

#if defined(OS_DARWIN) || defined(OS_FREEBSD)
#define HAVE_SOCKADDR_SA_LEN
#elif defined(OS_MINGW64_NT)
#ifndef WIN32
#define WIN32
#endif
#endif

#endif
