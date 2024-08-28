// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/get-context.h"        // get_context()
#include "network/context.h"            // Context
#include "network/contextinstance.h"    // ContextInstance
#include "network/optionalversion.h"    // OptionalVersion

#include <memory>       // std::make_shared(), std::shared_ptr

auto Network::get_context(const OptionalVersion& t_version,
                          bool t_is_verbose) -> ContextInstance
{
    static const auto context
    {
        std::make_shared<const Context>(t_version, t_is_verbose)
    };

    return context;
}
