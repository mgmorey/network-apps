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

#include "network/error.h"      // get_last_error(),
                                // reset_last_error(), std::string

#ifdef _WIN32
#include <winsock2.h>       // WSAGetLastError()
#include <windows.h>        // FORMAT_MESSAGE*, LANG_NEUTRAL,
                            // MAKELANGID(), SUBLANG_DEFAULT,
                            // FormatMessage(), LocalFree()
#else
#include <cerrno>       // errno
#include <cstring>      // std::strerror()
#endif

auto Network::format_error(error_type error) -> std::string
{
    std::string message;
#ifdef _WIN32
    LPVOID buffer {nullptr};
    const DWORD flags {
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS
    };
    const DWORD lang {MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)};
    const LPVOID pbuffer = &buffer;
    const LPTSTR pstring {static_cast<LPTSTR>(pbuffer)};

    if(::FormatMessage(flags, nullptr, error, lang, pstring, 0, nullptr)) {
        message = static_cast<LPTSTR>(buffer);
        const auto pos {message.find('\n')};
        message = message.substr(0, pos);
        ::LocalFree(buffer);
    }
#else
    message = std::strerror(error);
#endif
    return message;
}

auto Network::get_last_error() -> Network::error_type
{
    error_type error {0};
#ifdef _WIN32
    error = ::WSAGetLastError();
#else
    error = errno;
#endif
    return error;
}

auto Network::reset_last_error() -> Network::error_type
{
    error_type error {0};
#ifndef _WIN32
    errno = error;
#endif
    return error;
}
