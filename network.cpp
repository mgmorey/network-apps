#include "network.h"

#include <netdb.h>		// IPPROTO_TCP, struct addrinfo,
				// getaddrinfo(), getnameinfo()
#include <sys/socket.h>		// SOCK_STREAM, socklen_t
#include <unistd.h>		// gethostname()

#include <cstdlib>		// std::calloc()
#include <cstring>		// std::memcpy(), std::strdup()
#include <iostream>		// std::cerr, std::endl

AddrInfo::AddrInfo()
{
    init();
}

AddrInfo::AddrInfo(struct addrinfo& new_ai)
{
    init();
    copy(new_ai);
}

AddrInfo::AddrInfo(const AddrInfo& addrInfo)
{
    init();
    copy(addrInfo);
}

AddrInfo::~AddrInfo()
{
    cleanup();
}

AddrInfo& AddrInfo::operator=(const struct addrinfo& new_ai)
{
    cleanup();
    copy(new_ai);
    return *this;
}

AddrInfo& AddrInfo::operator=(const AddrInfo& addrInfo)
{
    ai = addrInfo.ai;
    return *this;
}

struct sockaddr* AddrInfo::getAddr() const
{
    return ai.ai_addr;
}

socklen_t AddrInfo::getAddrLen() const
{
    return ai.ai_addrlen;
}

void AddrInfo::copy(const struct addrinfo& new_ai)
{
    ai.ai_flags = new_ai.ai_flags;
    ai.ai_family = new_ai.ai_family;
    ai.ai_socktype = new_ai.ai_socktype;
    ai.ai_protocol = new_ai.ai_protocol;
    ai.ai_addrlen = new_ai.ai_addrlen;
    ai.ai_addr = static_cast<sockaddr*>
        (std::calloc(sizeof(char), ai.ai_addrlen));

    if (ai.ai_addr != NULL) {
        std::memcpy(ai.ai_addr, new_ai.ai_addr, ai.ai_addrlen);
    }

    if (new_ai.ai_canonname != NULL) {
        ai.ai_canonname = strdup(new_ai.ai_canonname);
    }
    else {
        ai.ai_canonname = NULL;
    }

    ai.ai_next = NULL;
}

void AddrInfo::cleanup()
{
    free(ai.ai_addr);
    free(ai.ai_canonname);
}

void AddrInfo::copy(const AddrInfo& addrInfo)
{
    copy(addrInfo.ai);
}

void AddrInfo::init()
{
    ai.ai_flags = 0;
    ai.ai_family = 0;
    ai.ai_socktype = 0;
    ai.ai_protocol = 0;
    ai.ai_addrlen = 0;
    ai.ai_addr = 0;
    ai.ai_canonname = NULL;
    ai.ai_next = NULL;
}

Addresses getAddresses(int family, int flags)
{
    return getAddresses(getHostname(), family, flags);
}

Addresses getAddresses(const Address& host, int family, int flags)
{
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
    AddrInfos addrInfos = getAddrInfo(host, &hints);
    Addresses addresses;

    for (AddrInfos::const_iterator it = addrInfos.begin();
         it != addrInfos.end();
         ++it) {
        Address address = getNameInfo(*it, NI_NUMERICHOST);
        addresses.insert(address);
    }

    return addresses;
}

AddrInfos getAddrInfo(const Address& host, struct addrinfo* hints)
{
    struct addrinfo* list = NULL;
    int error = getaddrinfo(host.c_str(), NULL, hints, &list);
    AddrInfos result;

    if (error) {
        std::cerr << "getaddrinfo() returned " << error
                  << " (" << gai_strerror(error)
                  << ')' << std::endl;
    }
    else {
        for (const struct addrinfo* elem = list;
             elem != NULL;
             elem = elem->ai_next) {
            struct addrinfo ai;
            ai.ai_flags = elem->ai_flags;
            ai.ai_family = elem->ai_family;
            ai.ai_socktype = elem->ai_socktype;
            ai.ai_protocol = elem->ai_protocol;
            ai.ai_addrlen = elem->ai_addrlen;
            ai.ai_addr = elem->ai_addr;
            ai.ai_canonname = NULL;
            ai.ai_next = NULL;
            result.push_back(ai);
        }

        freeaddrinfo(list);
    }

    return result;
}

HostName getHostname()
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

NameInfo getNameInfo(const AddrInfo& addrInfo, int flags)
{
    char* buffer = static_cast<char*>(std::calloc(sizeof(char), NI_MAXHOST));
    socklen_t length = sizeof(char) * NI_MAXHOST;
    int error = getnameinfo(addrInfo.getAddr(),
                            addrInfo.getAddrLen(),
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
