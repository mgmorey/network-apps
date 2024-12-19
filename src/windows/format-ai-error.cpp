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

#ifdef WIN32

#include "network/format-ai-error.h"    // format_ai_error()
#include "network/format-os-error.h"    // format_os_error()
#include "network/to-os-error.h"        // to_os_error()

#include <string>       // std::string

auto Network::format_ai_error(int error_code) -> std::string
{
    const auto os_error_code {to_os_error(error_code)};
    return format_os_error(os_error_code);
}

#endif
