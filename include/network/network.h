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
#include "network/ai-error.h"                   // format_ai_error()
#include "network/always-false.h"               // always_false_v
#include "network/binarybuffer.h"               // BinaryBuffer
#include "network/bind.h"                       // bind()
#include "network/buffer.h"                     // Buffer
#include "network/bytestring.h"                 // ByteString
#include "network/bytestringresult.h"           // ByteStringResult
#include "network/close.h"                      // close()
#include "network/connect.h"                    // connect()
#include "network/constants.h"                  // handle_null,
                                                // name_length_max,
                                                // name_length_min,
                                                // path_length_max,
                                                // path_length_min,
                                                // sa_data_offset,
                                                // sa_length_max,
                                                // sa_length_min,
                                                // sa_size,
                                                // sin6_addr_offset,
                                                // sin6_family_offset,
                                                // sin6_length_max,
                                                // sin6_length_min,
                                                // sin6_len_offset,
                                                // sin6_port_offset,
                                                // sin6_size,
                                                // sin_addr_offset,
                                                // sin_family_offset,
                                                // sin_length_max,
                                                // sin_length_min,
                                                // sin_len_offset,
                                                // sin_port_offset,
                                                // sin_size,
                                                // socket_error,
                                                // ss_size,
                                                // string_null,
                                                // sun_length_max,
                                                // sun_length_min,
                                                // sun_path_offset,
                                                // sun_size
#include "network/context-error.h"              // get_last_context_error()
                                                // reset_last_context_error()
                                                // set_last_context_error()
#include "network/context.h"                    // Context
#ifndef WIN32
#include "network/create-socketpair.h"          // create_socketpair()
#include "network/create-socketpairresult.h"    // create_socketpairresult()
#endif
#include "network/create-socket-handle.h"       // create_socket()
#include "network/create-socket-hints.h"        // create_socket()
#include "network/create-socket.h"              // create_socket()
#include "network/create-socketresult.h"        // create_result()
#include "network/error-strings.h"              // VISITOR_ERROR
#include "network/exceptions.h"                 // Error, LogicError,
                                                // RuntimeError
#include "network/failuremode.h"                // FailureMode
#include "network/get-endpoint.h"               // get_endpoint()
#include "network/get-endpointresult.h"         // get_endpointresult()
#include "network/get-hostname.h"               // get_hostname()
#include "network/get-hostnameresult.h"         // get_hostnameresult()
#include "network/get-name.h"                   // get_name()
#include "network/get-nameresult.h"             // get_nameresult()
#ifndef WIN32
#include "network/get-path-length.h"            // get_path_length()
#endif
#include "network/get-sa-family.h"              // get_sa_family()
#include "network/get-sa-length.h"              // get_sa_length()
#include "network/get-sa-pointer.h"             // get_sa_pointer()
#include "network/get-sin-pointer.h"            // get_sin_pointer()
#include "network/get-sin6-pointer.h"           // get_sin6_pointer()
#ifndef WIN32
#include "network/get-sun-length.h"             // get_sun_length()
#include "network/get-sun-pointer.h"            // get_sun_pointer()
#endif
#include "network/insert.h"                     // insert()
#include "network/ipsockethints.h"              // IpSocketHints
#include "network/listen.h"                     // listen()
#include "network/open-endpoint.h"              // open()
#include "network/open-handle.h"                // open()
#include "network/os-error.h"                   // format_os_error(),
                                                // get_last_os_error(),
                                                // reset_last_os_error()
#include "network/overloaded.h"                 // Overloaded
#include "network/quote.h"                      // quote()
#include "network/socket-error.h"               // socket_error
#include "network/socket.h"                     // Socket
#include "network/socketdata.h"                 // SocketData
#include "network/socketfamily.h"               // SocketFamily
#include "network/socketflags.h"                // SocketFlags
#include "network/sockethints.h"                // SocketHints
#include "network/sockethost.h"                 // SocketHost
#include "network/socketlimits.h"               // SocketLimits
#include "network/socketprotocol.h"             // SocketProtocol
#include "network/sockettype.h"                 // SocketType
#include "network/start-context.h"              // start_context()
#include "network/string-null.h"                // string_null
#include "network/template.h"                   // Template
#include "network/textbuffer.h"                 // TextBuffer
#include "network/to-bytestring.h"              // to_bytestring()
#include "network/to-name-length.h"             // to_name_length()
#include "network/to-os-error.h"                // to_os_error()
#ifndef WIN32
#include "network/to-path-length.h"             // to_path_length()
#include "network/to-path.h"                    // to_path()
#endif
#include "network/to-sa-length.h"               // to_sa_length()
#include "network/to-size.h"                    // to_size()
#include "network/to-socket-length.h"           // to_socket_length()
#include "network/to-string.h"                  // to_string()
#ifndef WIN32
#include "network/to-sun-length.h"              // to_sun_length()
#endif
#include "network/types.h"                      // Buffer, ByteString,
                                                // Hostname,
                                                // OptionalHostname,
                                                // OptionalPathname,
                                                // OptionalService,
                                                // Pathname, Service,
                                                // byte_type,
                                                // family_enum,
                                                // family_type,
                                                // handle_type,
                                                // length_type,
                                                // os_error_type,
                                                // port_type,
                                                // result_type,
                                                // socket_family_enum,
                                                // socket_family_type,
                                                // socket_flags_type,
                                                // socket_ip_family_enum,
                                                // socket_ip_protocol_enum,
                                                // socket_length_type,
                                                // socket_protocol_type,
                                                // socket_type_enum
                                                // socket_type_type
#include "network/uniquecontext.h"              // UniqueContext
#include "network/uniquesocket.h"               // UniqueSocket
#include "network/uniquify.h"                   // uniquify()
#include "network/validate.h"                   // validate()
#ifndef WIN32
#include "network/unixsockethints.h"            // UnixSocketHints
#endif
#include "network/version.h"                    // Version
#ifdef WIN32
#include "network/windowsversion.h"             // WindowsVersion
#endif

#endif
