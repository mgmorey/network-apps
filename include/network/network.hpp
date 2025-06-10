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

#ifndef NETWORK_NETWORK_HPP
#define NETWORK_NETWORK_HPP

#include "network/accept.hpp"                   // accept()
#include "network/address.hpp"                  // Address
#include "network/ai-error.hpp"                 // format_ai_error()
#include "network/always-false.hpp"             // always_false_v
#include "network/api-error.hpp"                // get_last_runtime_error()
                                                // reset_last_runtime_error()
                                                // set_last_runtime_error()
#include "network/apioptions.hpp"               // ApiOptions
#include "network/binarybuffer.hpp"             // BinaryBuffer
#include "network/bind.hpp"                     // bind()
#include "network/buffer.hpp"                   // Buffer
#include "network/bytestring.hpp"               // ByteString
#include "network/bytestringresult.hpp"         // ByteStringResult
#include "network/close.hpp"                    // close()
#include "network/connect.hpp"                  // connect()
#include "network/constants.hpp"                // handle_null,
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
#include "network/create-runtime.hpp"           // create_runtime()
#ifndef WIN32
#include "network/create-socketpair.hpp"        // create_socketpair()
#include "network/create-socketpairresult.hpp"  // create_socketpairresult()
#endif
#include "network/create-socket-handle.hpp"     // create_socket()
#include "network/create-socket-hints.hpp"      // create_socket()
#include "network/create-socket.hpp"            // create_socket()
#include "network/create-socketresult.hpp"      // create_result()
#include "network/error-strings.hpp"            // VISITOR_ERROR
#include "network/exceptions.hpp"               // Error, LogicError,
                                                // RuntimeError
#include "network/failmode.hpp"                 // FailMode
#include "network/get-endpoint.hpp"             // get_endpoint()
#include "network/get-endpointresult.hpp"       // get_endpointresult()
#include "network/get-hostname.hpp"             // get_hostname()
#include "network/get-hostnameresult.hpp"       // get_hostnameresult()
#include "network/get-name.hpp"                 // get_name()
#include "network/get-nameresult.hpp"           // get_nameresult()
#ifndef WIN32
#include "network/get-path-length.hpp"          // get_path_length()
#endif
#include "network/get-runtime.hpp"              // get_runtime()
#include "network/get-sa-family.hpp"            // get_sa_family()
#include "network/get-sa-length.hpp"            // get_sa_length()
#include "network/get-sa-pointer.hpp"           // get_sa_pointer()
#include "network/get-sin-pointer.hpp"          // get_sin_pointer()
#include "network/get-sin6-pointer.hpp"         // get_sin6_pointer()
#ifndef WIN32
#include "network/get-sun-length.hpp"           // get_sun_length()
#include "network/get-sun-pointer.hpp"          // get_sun_pointer()
#endif
#include "network/insert.hpp"                   // insert()
#include "network/ipsockethints.hpp"            // IpSocketHints
#include "network/listen.hpp"                   // listen()
#include "network/open-endpoint.hpp"            // open()
#include "network/open-handle.hpp"              // open()
#include "network/os-error.hpp"                 // format_os_error(),
                                                // get_last_os_error(),
                                                // reset_last_os_error()
#include "network/overloaded.hpp"               // Overloaded
#include "network/quote.hpp"                    // quote()
#include "network/run.hpp"                      // run()
#include "network/runtime.hpp"                  // Runtime
#include "network/sharedruntime.hpp"            // SharedRuntime
#include "network/socket-error.hpp"             // socket_error
#include "network/socket.hpp"                   // Socket
#include "network/socketdata.hpp"               // SocketData
#include "network/socketfamily.hpp"             // SocketFamily
#include "network/socketflags.hpp"              // SocketFlags
#include "network/sockethints.hpp"              // SocketHints
#include "network/sockethost.hpp"               // SocketHost
#include "network/socketlimits.hpp"             // SocketLimits
#include "network/socketprotocol.hpp"           // SocketProtocol
#include "network/sockettype.hpp"               // SocketType
#include "network/string-null.hpp"              // string_null
#include "network/template.hpp"                 // Template
#include "network/textbuffer.hpp"               // TextBuffer
#include "network/to-bytestring.hpp"            // to_bytestring()
#include "network/to-name-length.hpp"           // to_name_length()
#include "network/to-os-error.hpp"              // to_os_error()
#ifndef WIN32
#include "network/to-path-length.hpp"           // to_path_length()
#include "network/to-path.hpp"                  // to_path()
#endif
#include "network/to-sa-length.hpp"             // to_sa_length()
#include "network/to-size.hpp"                  // to_size()
#include "network/to-socket-length.hpp"         // to_socket_length()
#include "network/to-string.hpp"                // to_string()
#ifndef WIN32
#include "network/to-sun-length.hpp"            // to_sun_length()
#endif
#include "network/types.hpp"                    // Buffer, ByteString,
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
#include "network/uniquesocket.hpp"             // UniqueSocket
#include "network/uniquify.hpp"                 // uniquify()
#include "network/validate.hpp"                 // validate()
#ifndef WIN32
#include "network/unixsockethints.hpp"          // UnixSocketHints
#endif
#include "network/version.hpp"                  // Version
#ifdef WIN32
#include "network/windowsversion.hpp"           // WindowsVersion
#endif

#endif
