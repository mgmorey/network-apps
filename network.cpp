#include "network.h"

#include <netdb.h>	// struct addrinfo, getaddrinfo(), getnameinfo()
#include <unistd.h>	// gethostname()

#include <iostream>	// std::cerr, std::endl

Value getHostName(const struct addrinfo* ai, int flags)
{
    char* buffer = static_cast<char*>(calloc(sizeof(char), NI_MAXHOST));
    std::size_t size = sizeof(char) * NI_MAXHOST;
    int error = 0;

    if (ai == NULL) {
        error = gethostname(buffer, size);
    }
    else {
        error = getnameinfo(ai->ai_addr,
                            ai->ai_addrlen,
                            buffer, size,
                            NULL, 0,
                            flags);

        if (error) {
            std::cerr << "getnameinfo() returned " << error
                      << " (" << gai_strerror(error)
                      << ')' << std::endl;
        }
    }

    std::string host = buffer;
    free(buffer);
    return host;
}

Values getIpAddress(const Value& host)
{
    Values addrs;
    struct addrinfo* list = NULL;
    int error = getaddrinfo(host.c_str(), NULL, NULL, &list);

    if (error) {
        std::cerr << "getaddrinfo() returned " << error
                  << " (" << gai_strerror(error)
                  << ')' << std::endl;
        return addrs;
    }

    for (const struct addrinfo* elem = list; elem != NULL; elem = elem->ai_next)
        addrs.push_back(getHostName(elem, NI_NUMERICHOST));

    freeaddrinfo(list);
    return addrs;
}
