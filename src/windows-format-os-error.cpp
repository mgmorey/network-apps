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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>        // DWORD, FORMAT_MESSAGE_ALLOCATE_BUFFER,
                            // FORMAT_MESSAGE_FROM_SYSTEM,
                            // FORMAT_MESSAGE_IGNORE_INSERTS,
                            // LANG_NEUTRAL, LPTSTR, MAKELANGID(),
                            // SUBLANG_DEFAULT, ::FormatMessage(),
                            // ::LocalFree()

#include <string>       // std::string, std::to_string()

namespace {
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
}

auto Network::format_os_error(os_error_type os_error_code) -> std::string
{
    LPTSTR error_text {nullptr};

    if (format(os_error_code, error_text) == 0 || error_text == nullptr) {
        throw RuntimeError(std::string("Unable to format message for error ") +
                           std::to_string(os_error_code));
    }

    const std::string message {error_text};
    static_cast<void>(::LocalFree(error_text));
    return message.substr(0, message.rfind('\r'));
}
