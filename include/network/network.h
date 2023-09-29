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

#include "network/accept.h"                     // accept()
#include "network/address.h"                    // Address
#include "network/addressstring.h"              // AddressString
#include "network/ai-error.h"                   // format_ai_error()
#include "network/bind.h"                       // bind()
#include "network/bytestring.h"                 // ByteString
#include "network/bytestringresult.h"           // ByteStringResult
#include "network/close.h"                      // close()
#include "network/connect.h"                    // connect()
#include "network/constants.h"                  // descriptor_null,
                                                // sa_data_offset,
                                                // sa_size,
                                                // sin_addr_offset,
                                                // sin6_addr_offset,
                                                // sin_family_offset,
                                                // sin6_family_offset,
                                                // sin_len_offset,
                                                // sin6_len_offset,
                                                // sin_port_offset,
                                                // sin6_port_offset,
                                                // sin_size,
                                                // sin6_size,
                                                // sockaddr_size_max,
                                                // socket_error,
                                                // ss_size,
                                                // string_null,
                                                // sun_path_offset,
                                                // sun_size
#include "network/context.h"                    // Context
#include "network/create-socket.h"              // create_socket()
#include "network/create-socketpair.h"          // create_socketpair()
#include "network/create-socketpairresult.h"    // create_socketpairresult()
#include "network/create-socketresult.h"        // create_socketresult()
#include "network/exceptions.h"                 // Error, LogicError,
                                                // RuntimeError
#include "network/get-endpoint.h"               // get_endpoint()
#include "network/get-endpointresult.h"         // get_endpointresult()
#include "network/get-hostname.h"               // get_hostname()
#include "network/get-hostnameresult.h"         // get_hostnameresult()
#include "network/get-hosts.h"                  // get_hosts()
#include "network/get-length.h"                 // get_length()
#include "network/get-name.h"                   // get_name()
#include "network/get-nameresult.h"             // get_nameresult()
#include "network/get-path-length.h"            // get_path_length()
#include "network/get-path-size.h"              // get_path_size()
#include "network/get-peername.h"               // get_peername()
#include "network/get-peernameresult.h"         // get_peernameresult()
#include "network/get-sa-family.h"              // get_sa_family()
#include "network/get-sa-length.h"              // get_sa_length()
#include "network/get-sa-pointer.h"             // get_sa_pointer()
#include "network/get-sin-pointer.h"            // get_sin_pointer()
#include "network/get-sin6-pointer.h"           // get_sin6_pointer()
#include "network/get-sockname.h"               // get_sockname()
#include "network/get-socknameresult.h"         // get_socknameresult()
#include "network/get-sun-length.h"             // get_sun_length()
#include "network/get-sun-pointer.h"            // get_sun_pointer()
#include "network/get-templates.h"              // get_templates()
#include "network/is-valid.h"                   // is_valid()
#include "network/limits.h"                     // name_len_max,
                                                // name_len_min,
                                                // path_len_max,
                                                // path_len_min,
                                                // sock_len_max,
                                                // sock_len_min
#include "network/listen.h"                     // listen()
#include "network/open.h"                       // open()
#include "network/os-error.h"                   // format_os_error(),
                                                // get_last_os_error(),
                                                // reset_last_os_error()
#include "network/overloaded.h"                 // Overloaded
#include "network/read-string.h"                // read_string()
#include "network/socket-error.h"               // socket_error
#include "network/socket.h"                     // Socket
#include "network/socketfamily.h"               // SocketFamily
#include "network/socketflags.h"                // SocketFlags
#include "network/socketprotocol.h"             // SocketProtocol
#include "network/sockettype.h"                 // SocketType
#include "network/string-null.h"                // string_null
#include "network/template.h"                   // SocketHints,
                                                // SocketHost,
                                                // Template
#include "network/to-bytespan.h"                // to_bytespan()
#include "network/to-bytestring.h"              // to_bytestring()
#include "network/to-integer.h"                 // to_integer()
#include "network/to-name-len.h"                // to_name_len()
#include "network/to-os-error.h"                // to_os_error()
#include "network/to-path.h"                    // to_path()
#include "network/to-size.h"                    // to_size()
#include "network/to-sock-len.h"                // to_sock_len()
#include "network/to-string.h"                  // to_string()
#include "network/types.h"                      // Buffer, ByteString,
                                                // Hostname,
                                                // OptionalHostname,
                                                // OptionalPathname,
                                                // OptionalService,
                                                // Pathname, Service,
                                                // byte_type,
                                                // descriptor_type,
                                                // length_type,
                                                // os_error_type,
                                                // port_type,
                                                // result_type,
                                                // socket_family_type,
                                                // socket_flags_type,
                                                // socket_protocol_type,
                                                // socket_type_type,
                                                // sock_len_type
#include "network/uniquify.h"                   // uniquify()
#include "network/version-null.h"               // version_null
#include "network/version.h"                    // Version
#include "network/windowsversion.h"             // WindowsVersion
#include "network/write-string.h"               // write_string()

#endif
