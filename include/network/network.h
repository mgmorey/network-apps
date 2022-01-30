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

#ifndef NETWORK_NETWORK_H
#define NETWORK_NETWORK_H

#include "network/address.h"            // Address
#include "network/bind.h"               // bind()
#include "network/close.h"              // close()
#include "network/connect.h"            // connect()
#include "network/constants.h"          // fd_null, sa_data_offset,
                                        // sa_size, sin_addr_offset,
                                        // sin6_addr_offset,
                                        // sin_family_offset,
                                        // sin6_family_offset,
                                        // sin_len_offset,
                                        // sin6_len_offset,
                                        // sin_port_offset,
                                        // sin6_port_offset, sin_size,
                                        // sin6_size,
                                        // sockaddr_size_max,
                                        // socket_error, ss_size,
                                        // string_null,
                                        // sun_path_offset, sun_size
#include "network/context.h"            // Context
#include "network/exceptions.h"         // Error, LogicError,
                                        // RuntimeError
#include "network/family.h"             // Family
#include "network/flags.h"              // Flags
#include "network/get-endpoint.h"       // get_endpoint()
#include "network/get-hostname.h"       // get_hostname()
#include "network/get-hosts.h"          // get_hosts()
#include "network/get-length.h"         // get_length()
#include "network/get-peername.h"       // ByteStringResult,
                                        // get_peername()
#include "network/get-sa-family.h"      // get_sa_family()
#include "network/get-sa-length.h"      // get_sa_length()
#include "network/get-sa-pointer.h"     // get_sa_pointer()
#include "network/get-sin-pointer.h"    // get_sin_pointer()
#include "network/get-sin6-pointer.h"   // get_sin6_pointer()
#include "network/get-socket.h"         // get_socket()
#include "network/get-socketpair.h"     // get_socketpair()
#include "network/get-sockets.h"        // get_sockets()
#include "network/get-sockname.h"       // get_sockname()
#include "network/get-sun-length.h"     // get_sun_length()
#include "network/get-sun-path.h"       // get_sun_path()
#include "network/get-sun-pointer.h"    // get_sun_pointer()
#include "network/is-valid.h"           // is_valid()
#include "network/os-error.h"           // format_os_error(),
                                        // get_last_os_error(),
                                        // reset_last_os_error()
#include "network/overloaded.h"         // Overloaded
#include "network/protocol.h"           // Protocol
#include "network/result.h"             // Result
#include "network/skip-first.h"         // skip_first()
#include "network/skip-last.h"          // skip_last()
#include "network/socket-error.h"       // socket_error
#include "network/socket.h"             // Hints, Host, Socket
#include "network/socktype.h"           // SockType
#include "network/string-null.h"        // string_null
#include "network/to-byte-span.h"       // to_byte_span()
#include "network/to-byte-string.h"     // to_byte_string()
#include "network/to-string.h"          // to_string()
#include "network/types.h"              // Buffer, ByteString,
                                        // Hostname, OptionalHostname,
                                        // OptionalPathname,
                                        // OptionalService, Pathname,
                                        // Service, byte_type,
                                        // family_type, fd_type,
                                        // flags_type, length_type,
                                        // os_error_type, port_type,
                                        // protocol_type, result_type,
                                        // sock_len_type,
                                        // socktype_type
#include "network/uniquify.h"           // uniquify()
#include "network/version.h"            // Version

#endif
