#include "network.h"

#include <sys/socket.h>		// SOCK_STREAM, socklen_t
#include <netdb.h>		// IPPROTO_TCP, struct addrinfo,
				// getaddrinfo(), getnameinfo()
#include <unistd.h>		// gethostname()

#include <cstdlib>		// std::calloc()
#include <iostream>		// std::cerr, std::endl

Addresses getAddresses(int family, int flags)
{
    return getAddresses(getHostname(), family, flags);
}

Addresses getAddresses(const Address& host, int family, int flags)
{
    Addresses addrs;
    struct addrinfo* list = NULL;
    struct addrinfo hints = {
        flags,
        family,
        0,
        0,
        0,
        NULL,
        NULL,
        NULL
    };
    int error = getaddrinfo(host.c_str(), NULL, &hints, &list);

    if (error) {
        std::cerr << "getaddrinfo() returned " << error
                  << " (" << gai_strerror(error)
                  << ')' << std::endl;
        return addrs;
    }

    for (const struct addrinfo* elem = list; elem != NULL; elem = elem->ai_next)
        addrs.insert(getNameInfo(*elem, NI_NUMERICHOST));

    freeaddrinfo(list);
    return addrs;
}

Address getHostname()
{
    char* buffer = static_cast<char*>(std::calloc(sizeof(char), NI_MAXHOST));
    std::size_t length = sizeof(char) * NI_MAXHOST;
    int error = gethostname(buffer, length - 1);

    if (error) {
        std::cerr << "gethostname() returned " << error << std::endl;
    }

    std::string host = buffer;
    free(buffer);
    return host;
}

Address getNameInfo(const struct addrinfo& ai, int flags)
{
    char* buffer = static_cast<char*>(std::calloc(sizeof(char), NI_MAXHOST));
    socklen_t length = sizeof(char) * NI_MAXHOST;
    int error = getnameinfo(ai.ai_addr,
                            ai.ai_addrlen,
                            buffer, length,
                            NULL, 0,
                            flags);

    if (error) {
        std::cerr << "getnameinfo() returned " << error
                  << " (" << gai_strerror(error)
                  << ')' << std::endl;
    }

    std::string host = buffer;
    free(buffer);
    return host;
}
