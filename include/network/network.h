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
#include "network/addrinfo.h"           // AddrInfo
#include "network/bind.h"               // bind()
#include "network/bytes.h"              // Bytes
#include "network/close.h"              // close()
#include "network/connect.h"            // connect()
#include "network/context.h"            // Context
#include "network/get-endpoint.h"       // EndpointResult,
                                        // get_endpoint()
#include "network/get-hostname.h"       // HostnameResult,
                                        // get_hostname()
#include "network/get-hosts.h"          // HostVectorResult,
                                        // get_hosts()
#include "network/get-length.h"         // get_length()
#include "network/get-peername.h"       // BytesResult,
                                        // get_peername()
#include "network/get-sa-family.h"      // get_sa_family()
#include "network/get-sa-length.h"      // get_sa_length()
#include "network/get-sa-pointer.h"     // get_sa_pointer()
#include "network/get-sin-pointer.h"    // get_sin_pointer()
#include "network/get-sin6-pointer.h"   // get_sin6_pointer()
#include "network/get-sockaddr.h"       // get_sockaddr()
#include "network/get-socket.h"         // Fd, FdResult, Socket,
                                        // get_socket()
#include "network/get-socketpair.h"     // FdPair, FdPairResult,
                                        // get_socketpair()
#include "network/get-sockets.h"        // SocketVectorResult,
                                        // get_sockets()
#include "network/get-sockname.h"       // BytesResult,
                                        // get_sockname()
#include "network/get-sun-length.h"     // get_sun_length()
#include "network/get-sun-path.h"       // get_sun_path()
#include "network/get-sun-pointer.h"    // get_sun_pointer()
#include "network/is-valid.h"           // is_valid()
#include "network/overload.h"           // Overload
#include "network/result.h"             // Result
#include "network/socket.h"             // Hints, Host, Socket
#include "network/types.h"              // Buffer, Hostname, Pathname
                                        // Service

#endif
