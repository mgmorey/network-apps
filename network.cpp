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

AddrInfo::AddrInfo(struct addrinfo& ai)
{
    setup();
    copy(ai);
}

AddrInfo::AddrInfo(const AddrInfo& ai)
{
    setup();
    copy(ai);
}

AddrInfo::~AddrInfo()
{
    reset();
}

AddrInfo& AddrInfo::operator=(const struct addrinfo& ai)
{
    reset();
    copy(ai);
    return *this;
}

AddrInfo& AddrInfo::operator=(const AddrInfo& ai)
{
    this->ai = ai.ai;
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

void AddrInfo::copy(const struct addrinfo& ai)
{
    this->ai.ai_flags = ai.ai_flags;
    this->ai.ai_family = ai.ai_family;
    this->ai.ai_socktype = ai.ai_socktype;
    this->ai.ai_protocol = ai.ai_protocol;
    this->ai.ai_addrlen = ai.ai_addrlen;
    this->ai.ai_addr = static_cast<sockaddr*>
        (std::calloc(sizeof(char), this->ai.ai_addrlen));

    if (this->ai.ai_addr != NULL) {
        std::memcpy(this->ai.ai_addr, ai.ai_addr, ai.ai_addrlen);
    }

    if (ai.ai_canonname != NULL) {
        this->ai.ai_canonname = strdup(ai.ai_canonname);
    }
    else {
        this->ai.ai_canonname = NULL;
    }
}

void AddrInfo::copy(const AddrInfo& ai)
{
    copy(ai.ai);
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
