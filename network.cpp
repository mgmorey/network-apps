#include "network.h"

#include <netdb.h>	// struct addrinfo, getaddrinfo(), getnameinfo()
#include <unistd.h>	// gethostname()

#include <climits>	// HOST_SIZE_MAX
#include <iostream>	// std::cerr, std::endl

Value getHostName(const struct addrinfo* ai, int flags)
{
    char host[HOST_NAME_MAX] = "";

    if (ai == NULL)
        return gethostname(host, sizeof host) == 0 ? host : "";

    int error = getnameinfo(ai->ai_addr,
                            ai->ai_addrlen,
                            host, sizeof host,
                            NULL, 0,
                            flags);

    if (error)
        std::cerr << "getnameinfo() returned " << error
                  << " (" << gai_strerror(error)
                  << ')' << std::endl;

    return error == 0 ? host : "";
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
