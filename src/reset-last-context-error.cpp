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

#include "network/reset-last-context-error.h"   // reset_last_context_error()
#include "network/context-error-type.h"         // context_error_type
#include "network/set-last-context-error.h"     // set_last_context_error()

auto Network::reset_last_context_error() -> Network::context_error_type
{
    const context_error_type error {0};
    set_last_context_error(error);
    return error;
}
