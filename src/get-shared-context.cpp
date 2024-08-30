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

#include "network/get-shared-context.h" // get_shared_context()
#include "network/failuremode.h"        // FailureMode
#include "network/optionalversion.h"    // OptionalVersion
#include "network/sharedcontext.h"      // SharedContext

#ifdef WIN32
#include "network/windowscontext.h"     // WindowsContext
#else
#include "network/unixcontext.h"        // UnixContext
#endif

#include <memory>       // std::make_shared()

auto Network::get_shared_context(const OptionalVersion& t_version,
                                 FailureMode t_failure,
                                 bool t_is_verbose) -> SharedContext
{
    static const auto context
    {
#ifdef WIN32
        std::make_shared<WindowsContext>(t_version, t_failure, t_is_verbose)
#else
        std::make_shared<UnixContext>(t_version, t_failure, t_is_verbose)
#endif
    };

    if (context) {
        context->start();
    }

    return context;
}

auto Network::get_shared_context(bool t_is_verbose) -> SharedContext
{
    return get_shared_context({}, FailureMode::throw_error, t_is_verbose);
}
