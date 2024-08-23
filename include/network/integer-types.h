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

#ifndef NETWORK_INTEGER_TYPES_H
#define NETWORK_INTEGER_TYPES_H

#include "network/byte-type.h"                      // byte_type
#include "network/context-error-type.h"             // context_error_type
#include "network/error-type.h"                     // error_type
#include "network/handle-type.h"                    // handle_type
#include "network/length-type.h"                    // length_type
#include "network/name-length-type.h"               // name_length_type
#include "network/os-error-type.h"                  // os_error_type
#include "network/port-type.h"                      // port_type
#include "network/sa-family-type.h"                 // sa_family_type
#include "network/sin-family-type.h"                // sin_family_type
#include "network/socket-family-type.h"             // socket_family_type
#include "network/socket-flags-type.h"              // socket_flags_type
#include "network/socket-hint-type.h"               // socket_hint_type
#include "network/socket-length-type.h"             // socket_length_type
#include "network/socket-protocol-type.h"           // socket_protocol_type
#include "network/socket-type-type.h"               // socket_type_type
#ifdef WIN32
#include "network/windows-version-type.h"           // windows_version_type
#endif

#endif
