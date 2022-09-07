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

#include "network/startup.h"            // Context, OptionalVersion,
                                        // Version, startup()
#include "network/exceptions.h"         // Error, LogicError,
                                        // RunTimeError
#include "network/format-os-error.h"    // format_os_error()
#include "network/windowsversion.h"     // Version, WORD, WSADATA,
                                        // WSAEFAULT, WSAEPROCLIM,
                                        // WSASYSNOTREADY,
                                        // WSAVERNOTSUPPORTED,
                                        // WSAStartup(),
                                        // WindowsVersion

#ifdef WIN32
static constexpr auto version_default {Network::Version {2, 2}};
#else
static constexpr auto version_default {Network::Version {0, 0}};
#endif

auto Network::startup(Context& context, const OptionalVersion& version) -> void
{
#ifdef WIN32
    WSADATA wsa_data {};
    const WindowsVersion required_version {version.value_or(version_default)};
    const auto error_code {::WSAStartup(WORD {required_version}, &wsa_data)};

    if (error_code != 0) {
        const auto error_str {format_os_error(error_code)};

        switch (error_code) {  // NOLINT
        case WSAEFAULT:
        case WSAVERNOTSUPPORTED:
            throw LogicError {error_str};
            break;
        case WSAEPROCLIM:
        case WSASYSNOTREADY:
            throw RuntimeError {error_str};
            break;
        default:
            throw Error {error_str};
        }
    }

    context.description(static_cast<const char*>(wsa_data.szDescription))
        .system_status(static_cast<const char*>(wsa_data.szSystemStatus))
        .version(Version {WindowsVersion {wsa_data.wVersion}});
#else
    const auto required_version {version.value_or(version_default)};
    context.description("Berkeley Software Distribution Sockets")
        .version(required_version);
#endif
    context.is_started(true);
}
