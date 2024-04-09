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

#include "network/format-os-error.h"    // format_os_error()
#include "network/os-error-type.h"      // os_error_type
#include "network/runtimeerror.h"       // RuntimeError

#ifdef WIN32
#include <winsock2.h>       // Always include winsock2.h before
                            // windows.h on Windows
#include <windows.h>        // DWORD, FORMAT_MESSAGE_ALLOCATE_BUFFER,
                            // FORMAT_MESSAGE_FROM_SYSTEM,
                            // FORMAT_MESSAGE_IGNORE_INSERTS,
                            // LANG_NEUTRAL, LPTSTR, MAKELANGID(),
                            // SUBLANG_DEFAULT, ::FormatMessage(),
                            // ::LocalFree()
#else
#include <cstring>          // std::strerror()
#endif

#include <string>       // std::string, std::to_string()

namespace {
#ifdef WIN32
    auto format(DWORD error, LPTSTR& error_text) -> DWORD
    {
        return ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
                               FORMAT_MESSAGE_ALLOCATE_BUFFER |
                               FORMAT_MESSAGE_IGNORE_INSERTS,
                               nullptr,
                               error,
                               MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                               reinterpret_cast<LPTSTR>(&error_text),  // NOLINT
                               0,
                               nullptr);
    }
#endif
}

auto Network::format_os_error(os_error_type error) -> std::string
{
#ifdef WIN32
    LPTSTR error_text {nullptr};

    if (format(error, error_text) == 0 || error_text == nullptr) {
        throw RuntimeError(std::string("Unable to format message for error ") +
                           std::to_string(error));
    }

    std::string message {error_text};
    const auto pos {message.rfind('\r')};

    if (pos != std::string::npos) {
        message.resize(pos);
    }

    ::LocalFree(error_text);
    return message;
#else
    return std::strerror(error);
#endif
}
