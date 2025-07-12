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

#ifndef NETWORK_OS_FEATURES_HPP
#define NETWORK_OS_FEATURES_HPP

#if defined(__APPLE__) && defined(__MACH__)
#define HAVE_SOCKADDR_SA_LEN
#elif defined(__unix__) && !defined(__linux__) && !defined(__CYGWIN__)
#define HAVE_SOCKADDR_SA_LEN
#endif

#endif
