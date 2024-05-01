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
#include "network/always-false.h"               // always_false_v
#include "network/bind.h"                       // bind()
#include "network/bytestring.h"                 // ByteString
#include "network/bytestringresult.h"           // ByteStringResult
#include "network/close.h"                      // close()
#include "network/connect.h"                    // connect()
#include "network/constants.h"                  // descriptor_null,
                                                // sa_data_offset,
                                                // sa_size,
                                                // sin6_addr_offset,
                                                // sin6_family_offset,
                                                // sin6_len_offset,
                                                // sin6_port_offset,
                                                // sin6_size,
                                                // sin_addr_offset,
                                                // sin_family_offset,
                                                // sin_len_offset,
                                                // sin_port_offset,
                                                // sin_size,
                                                // sockaddr_size_max,
                                                // socket_error,
                                                // ss_size,
                                                // string_null,
                                                // sun_path_offset,
                                                // sun_size
#include "network/context.h"                    // Context
#ifndef WIN32
#include "network/create-pair.h"                // create_pair()
#include "network/create-pairresult.h"          // create_pairresult()
#endif
#include "network/create-result.h"              // create_result()
#include "network/create.h"                     // create()
#include "network/error-strings.h"              // VISITOR_ERROR
#include "network/exceptions.h"                 // Error, LogicError,
                                                // RuntimeError
#include "network/get-endpoint.h"               // get_endpoint()
#include "network/get-endpointresult.h"         // get_endpointresult()
#include "network/get-hostname.h"               // get_hostname()
#include "network/get-hostnameresult.h"         // get_hostnameresult()
#include "network/get-hosts.h"                  // get_hosts()
#include "network/get-length-maximum.h"         // get_length_maximum()
#include "network/get-length-minimum.h"         // get_length_minimum()
#include "network/get-name.h"                   // get_name()
#include "network/get-nameresult.h"             // get_nameresult()
#ifndef WIN32
#include "network/get-path-length.h"            // get_path_length()
#endif
#include "network/get-peername.h"               // get_peername()
#include "network/get-peernameresult.h"         // get_peernameresult()
#include "network/get-sa-family.h"              // get_sa_family()
#include "network/get-sa-length.h"              // get_sa_length()
#include "network/get-sa-pointer.h"             // get_sa_pointer()
#include "network/get-sin-pointer.h"            // get_sin_pointer()
#include "network/get-sin6-pointer.h"           // get_sin6_pointer()
#include "network/get-sockname.h"               // get_sockname()
#include "network/get-socknameresult.h"         // get_socknameresult()
#ifndef WIN32
#include "network/get-sun-length.h"             // get_sun_length()
#include "network/get-sun-pointer.h"            // get_sun_pointer()
#endif
#include "network/get-templates.h"              // get_templates()
#include "network/ipsockethints.h"              // IpSocketHints
#include "network/limits.h"                     // name_length_max,
                                                // name_length_min,
                                                // path_length_max,
                                                // path_length_min,
#include "network/listen.h"                     // listen()
#include "network/open-endpoint.h"              // open()
#include "network/open-socket.h"                // open()
#include "network/os-error.h"                   // format_os_error(),
                                                // get_last_os_error(),
                                                // reset_last_os_error()
#include "network/overloaded.h"                 // Overloaded
#include "network/read-string.h"                // read_string()
#include "network/remove.h"                     // remove()
#include "network/socket-error.h"               // socket_error
#include "network/socket.h"                     // Socket
#include "network/socketfamily.h"               // SocketFamily
#include "network/socketflags.h"                // SocketFlags
#include "network/sockethints.h"                // SocketHints
#include "network/sockethost.h"                 // SocketHints
#include "network/socketprotocol.h"             // SocketProtocol
#include "network/sockettype.h"                 // SocketType
#include "network/string-null.h"                // string_null
#include "network/template.h"                   // Template
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
                                                // descriptor_type,
                                                // length_type,
                                                // os_error_type,
                                                // port_type,
                                                // result_type,
                                                // socket_family_type,
                                                // socket_flags_type,
                                                // socket_length_type,
                                                // socket_protocol_type,
                                                // socket_type_type
#include "network/uniquify.h"                   // uniquify()
#include "network/unixsockethints.h"            // UnixSocketHints
#include "network/version-null.h"               // version_null
#include "network/version.h"                    // Version
#include "network/windowsversion.h"             // WindowsVersion
#include "network/write-string.h"               // write_string()

#endif
