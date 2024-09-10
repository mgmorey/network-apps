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
#include "network/failuremode.h"        // FailureMode
#include "network/optionalversion.h"    // OptionalVersion
#include "network/sharedcontext.h"      // SharedContext

#ifdef WIN32
#include "network/windowscontext.h"     // WindowsContext
#else
#include "network/unixcontext.h"        // UnixContext
#endif

#include <memory>       // std::make_shared()

auto Network::get_context(const OptionalVersion& t_version,
                          FailureMode t_failure,
                          bool t_is_verbose) -> Network::SharedContext
{
#ifdef WIN32
    return std::make_shared<Network::WindowsContext>(t_version,
                                                     t_failure,
                                                     t_is_verbose);
#else
    return std::make_shared<Network::UnixContext>(t_version,
                                                  t_failure,
                                                  t_is_verbose);
#endif
}

auto Network::get_context(const OptionalVersion& t_version,
                          FailureMode t_failure,
                          bool t_is_global,
                          bool t_is_verbose) -> SharedContext
{
    SharedContext context;

    if (t_is_global) {
        static const auto global_context =
            get_context(t_version, t_failure, t_is_verbose);
        context = global_context;
    }
    else {
        context =
            get_context(t_version, t_failure, t_is_verbose);
    }

    if (context) {
        context->start();
    }

    return context;
}

auto Network::get_context(bool t_is_global, bool t_is_verbose) -> SharedContext
{
    return get_context({}, FailureMode::throw_error, t_is_global, t_is_verbose);
}
