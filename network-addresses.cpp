#include "network-addresses.h"
#include "network-addrinfo.h"

#ifdef _WIN32
#include <winsock2.h>		// closesocket(), connect(),
                                // gethostname(), socket()
#include <ws2tcpip.h>		// NI_MAXHOST, struct addrinfo,
                                // getaddrinfo(), getnameinfo(),
                                // socklen_t
#else
#include <netdb.h>		// NI_MAXHOST, struct addrinfo,
                                // getaddrinfo(), getnameinfo()
#include <sys/socket.h>		// AF_UNSPEC, connect(), socket(),
                                // socklen_t
#include <unistd.h>		// close(), gethostname()
#endif

#include <cassert>		// assert()
#include <cerrno>		// errno
#include <cstdlib>		// free()
#include <cstring>		// std::memcpy(), strdup(),
                                // std::strerror()
#include <iostream>		// std::cerr, std::endl

Addresses get_addresses(const std::string& node,
                        const struct addrinfo* hints)
{
    Addresses result;
    copy_addrinfo(result, node, "", hints);
    result.unique();
    return result;
}

Addresses get_addresses(const std::string& node,
                        const struct addrinfo& hints)
{
    return get_addresses(node, &hints);
}
