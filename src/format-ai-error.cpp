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

#include "network/format-ai-error.h"    // error_type,
                                        // format_ai_error(),
                                        // std::string
#include "network/format-os-error.h"    // format_os_error()
#include "network/os-features.h"        // WIN32

#ifdef WIN32
#include <ws2tcpip.h>       // gai_strerror()
#else
#include <netdb.h>          // gai_strerror()
#endif

auto Network::format_ai_error(error_type error) noexcept -> std::string
{
#ifdef WIN32
    return format_os_error(error);
#else
    return ::gai_strerror(error);
#endif
}
