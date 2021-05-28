#include "network.h"

#include <netdb.h>		// IPPROTO_TCP, struct addrinfo,
				// getaddrinfo(), getnameinfo()
#include <sys/socket.h>		// SOCK_STREAM, socklen_t
#include <unistd.h>		// gethostname()

#include <cstdlib>		// std::calloc()
#include <cstring>		// std::memcpy(), std::strdup()
#include <iostream>		// std::cerr, std::endl

Addrinfo::Addrinfo()
{
    set_up();
}

Addrinfo::Addrinfo(struct addrinfo& ai)
{
    set_up();
    copy(ai);
}

Addrinfo::Addrinfo(const Addrinfo& ai)
{
    set_up();
    copy(ai);
}

Addrinfo::~Addrinfo()
{
    reset();
}

Addrinfo& Addrinfo::operator=(const struct addrinfo& ai)
{
    reset();
    copy(ai);
    return *this;
}

Addrinfo& Addrinfo::operator=(const Addrinfo& ai)
{
    this->ai = ai.ai;
    return *this;
}

const struct sockaddr* Addrinfo::get_addr() const
{
    return ai.ai_addr;
}

socklen_t Addrinfo::get_addrlen() const
{
    return ai.ai_addrlen;
}

const char* Addrinfo::get_canonname() const
{
    return ai.ai_canonname;
}

int Addrinfo::get_family() const
{
    return ai.ai_family;
}

int Addrinfo::get_flags() const
{
    return ai.ai_flags;
}

int Addrinfo::get_protocol() const
{
    return ai.ai_protocol;
}

int Addrinfo::get_socktype() const
{
    return ai.ai_socktype;
}

void Addrinfo::copy(const struct addrinfo& ai)
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

void Addrinfo::copy(const Addrinfo& ai)
{
    copy(ai.ai);
}

void Addrinfo::reset()
{
    free(ai.ai_addr);
    ai.ai_addr = NULL;
    free(ai.ai_canonname);
    ai.ai_canonname = NULL;
}

void Addrinfo::set_up()
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

Addresses get_addresses(int family, int flags)
{
    return get_addresses(get_hostname(), family, flags);
}

Addresses get_addresses(const Address& host, int family, int flags)
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
    Addrinfos ai = get_addrinfo(host, &hints);
    Addresses result;

    for (Addrinfos::const_iterator it = ai.begin();
         it != ai.end();
         ++it) {
        Nameinfo nameInfo = get_nameinfo(*it, NI_NUMERICHOST);
        result.insert(nameInfo.first);
    }

    return result;
}

Addrinfos get_addrinfo(const Address& host, struct addrinfo* hints)
{
    struct addrinfo* list = NULL;
    int error = getaddrinfo(host.c_str(), NULL, hints, &list);
    Addrinfos result;

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

Hostname get_hostname()
{
    char* host = static_cast<char*>(std::calloc(sizeof(char), NI_MAXHOST));
    int error = gethostname(host, NI_MAXHOST - 1);

    if (error) {
        std::cerr << "gethostname() returned " << error << std::endl;
    }

    std::string result = host;
    free(host);
    return result;
}

Nameinfo get_nameinfo(const Addrinfo& ai, int flags)
{
    char* host = static_cast<char*>(std::calloc(sizeof(char), NI_MAXHOST));
    char* serv = static_cast<char*>(std::calloc(sizeof(char), NI_MAXHOST));
    int error = getnameinfo(ai.get_addr(),
                            ai.get_addrlen(),
                            host, NI_MAXHOST,
                            serv, NI_MAXHOST,
                            flags);

    if (error) {
        std::cerr << "getnameinfo() returned " << error
                  << " (" << gai_strerror(error)
                  << ')' << std::endl;
    }

    Nameinfo result = Nameinfo(host, serv);
    free(serv);
    free(host);
    return result;
}
