#include "network-addrinfo.h"

#ifdef _WIN32
#include <winsock2.h>		// struct sockaddr
#include <ws2tcpip.h>		// struct addrinfo
#else
#include <netdb.h>		// struct addrinfo
#include <sys/socket.h>		// struct sockaddr
#endif

#include <cstdlib>		// free()
#include <cstring>		// std::memcpy(), strdup()

void copy_addrinfo(struct addrinfo& dest, const struct addrinfo& src)
{
    dest.ai_flags = src.ai_flags;
    dest.ai_family = src.ai_family;
    dest.ai_socktype = src.ai_socktype;
    dest.ai_protocol = src.ai_protocol;
    dest.ai_addrlen = src.ai_addrlen;
    free(dest.ai_canonname);
    dest.ai_canonname = NULL;

    if (src.ai_canonname != NULL) {
        dest.ai_canonname = strdup(src.ai_canonname);
    }

    free(dest.ai_addr);
    dest.ai_addr = NULL;

    if (src.ai_addr != NULL) {
        dest.ai_addr = static_cast<struct sockaddr*>(malloc(dest.ai_addrlen));
        std::memcpy(dest.ai_addr, src.ai_addr, dest.ai_addrlen);
    }

    dest.ai_next = NULL;
}

void init_addrinfo(struct addrinfo& dest,
                   int protocol,
                   int socktype,
                   int family,
                   int flags)
{
    std::memset(&dest, '\0', sizeof dest);
    dest.ai_flags = flags;
    dest.ai_family = family;
    dest.ai_socktype = socktype;
    dest.ai_protocol = protocol;
    dest.ai_addrlen = 0;
    dest.ai_canonname = NULL;
    dest.ai_addr = NULL;
    dest.ai_next = NULL;
}
