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
    setup();
}

AddrInfo::AddrInfo(struct addrinfo& newAi)
{
    setup();
    copy(newAi);
}

AddrInfo::AddrInfo(const AddrInfo& addrInfo)
{
    setup();
    copy(addrInfo);
}

AddrInfo::~AddrInfo()
{
    reset();
}

AddrInfo& AddrInfo::operator=(const struct addrinfo& newAi)
{
    reset();
    copy(newAi);
    return *this;
}

AddrInfo& AddrInfo::operator=(const AddrInfo& addrInfo)
{
    ai = addrInfo.ai;
    return *this;
}

const struct sockaddr* AddrInfo::getAddr() const
{
    return ai.ai_addr;
}

socklen_t AddrInfo::getAddrLen() const
{
    return ai.ai_addrlen;
}

const char* AddrInfo::getCanonName() const
{
    return ai.ai_canonname;
}

int AddrInfo::getFamily() const
{
    return ai.ai_family;
}

int AddrInfo::getFlags() const
{
    return ai.ai_flags;
}

int AddrInfo::getProtocol() const
{
    return ai.ai_protocol;
}

int AddrInfo::getSockType() const
{
    return ai.ai_socktype;
}

void AddrInfo::copy(const struct addrinfo& newAi)
{
    ai.ai_flags = newAi.ai_flags;
    ai.ai_family = newAi.ai_family;
    ai.ai_socktype = newAi.ai_socktype;
    ai.ai_protocol = newAi.ai_protocol;
    ai.ai_addrlen = newAi.ai_addrlen;
    ai.ai_addr = static_cast<sockaddr*>
        (std::calloc(sizeof(char), ai.ai_addrlen));

    if (ai.ai_addr != NULL) {
        std::memcpy(ai.ai_addr, newAi.ai_addr, ai.ai_addrlen);
    }

    if (newAi.ai_canonname != NULL) {
        ai.ai_canonname = strdup(newAi.ai_canonname);
    }
    else {
        ai.ai_canonname = NULL;
    }
}

void AddrInfo::copy(const AddrInfo& addrInfo)
{
    copy(addrInfo.ai);
}

void AddrInfo::reset()
{
    free(ai.ai_addr);
    ai.ai_addr = NULL;
    free(ai.ai_canonname);
    ai.ai_canonname = NULL;
}

void AddrInfo::setup()
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
        NameInfo nameInfo = getNameInfo(*it, NI_NUMERICHOST);
        addresses.insert(nameInfo.first);
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
    char* host = static_cast<char*>(std::calloc(sizeof(char), NI_MAXHOST));
    char* serv = static_cast<char*>(std::calloc(sizeof(char), NI_MAXHOST));
    int error = getnameinfo(addrInfo.getAddr(),
                            addrInfo.getAddrLen(),
                            host, NI_MAXHOST,
                            serv, NI_MAXHOST,
                            flags);

    if (error) {
        std::cerr << "getnameinfo() returned " << error
                  << " (" << gai_strerror(error)
                  << ')' << std::endl;
    }

    NameInfo result = NameInfo(host, serv);
    free(serv);
    free(host);
    return result;
}
