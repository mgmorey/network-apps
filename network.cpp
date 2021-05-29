#include "network.h"

#ifdef _WIN32
#include <winsock2.h>		// AF_UNSPEC, gethostname()
#else
#include <netdb.h>		// NI_MAXHOST, struct addrinfo
#include <sys/socket.h>		// socket definitions
#include <unistd.h>		// Unix functions
#endif

#include <cstdlib>		// std::calloc()
#include <iostream>		// std::cerr, std::endl

Address::Address()
{
    set_up();
}

Address::Address(const Address& address)
{
    set_up();
    copy(address);
}

Address::Address(const struct addrinfo& ai)
{
    set_up();
    copy(ai);
}

Address& Address::operator=(const Address& address)
{
    copy(address);
    return *this;
}

Address& Address::operator=(const struct addrinfo& ai)
{
    copy(ai);
    return *this;
}

bool Address::operator<(const Address& address) const
{
    return (family < address.family ||
            protocol < address.protocol ||
            socktype < address.socktype ||
            addr < address.addr);
}

bool Address::operator>(const Address& address) const
{
    return (family > address.family ||
            protocol > address.protocol ||
            socktype > address.socktype ||
            addr > address.addr);
}

bool Address::operator!=(const Address& address) const
{
    return (family != address.family ||
            protocol != address.protocol ||
            socktype != address.socktype ||
            addr != address.addr);
}

bool Address::operator==(const Address& address) const
{
    return (family == address.family &&
            protocol == address.protocol &&
            socktype == address.socktype &&
            addr == address.addr);
}

const sockaddr* Address::get_addr() const
{
    return reinterpret_cast<const sockaddr*>(addr.data());
}

socklen_t Address::get_addrlen() const
{
    return addr.size();
}

int Address::get_family() const
{
    return family;
}

int Address::get_protocol() const
{
    return protocol;
}

int Address::get_socktype() const
{
    return socktype;
}

std::string Address::to_string() const
{
    char* host = static_cast<char*>(std::calloc(sizeof(char), NI_MAXHOST));
    int error = getnameinfo(get_addr(), get_addrlen(),
                            host, NI_MAXHOST,
                            NULL, 0,
                            NI_NUMERICHOST);

    if (error) {
        std::cerr << "getnameinfo() returned "
                  << error
                  << " ("
                  << gai_strerror(error)
                  << ')'
                  << std::endl;
    }

    std::string result = host;
    free(host);
    return result;
}

void Address::copy(const Address& address)
{
    addr = address.addr;
    family = address.family;
    protocol = address.protocol;
    socktype = address.socktype;
}

void Address::copy(const struct addrinfo& ai)
{
    addr.clear();
    addr.append(reinterpret_cast<const char*>(ai.ai_addr), ai.ai_addrlen);
    family = ai.ai_family;
    protocol = ai.ai_protocol;
    socktype = ai.ai_socktype;
}

void Address::set_up()
{
    family = 0;
    protocol = 0;
    socktype = 0;
}

Addresses get_addresses(const std::string& host, int family, int flags)
{
    struct addrinfo hints = {
        flags,		// ai_flags
        family,		// ai_family
        0,		// ai_socktype
        0,		// ai_protocol
        0,		// ai_addrlen
        NULL,		// ai_canonname
        NULL,		// ai_addr
        NULL		// ai_next
    };
    Addrinfos ai = get_addrinfo(host, "", &hints);
    Addresses result;

    for (Addrinfos::const_iterator it = ai.begin();
         it != ai.end();
         ++it) {
        result.insert(it->first);
    }

    return result;
}

Addrinfos get_addrinfo(const std::string& node,
                       const std::string& service,
                       struct addrinfo* hints)
{
    struct addrinfo* list = NULL;
    int error = getaddrinfo(node.empty() ? NULL : node.c_str(),
                            service.empty() ? NULL : service.c_str(),
                            hints,
                            &list);
    Addrinfos result;

    if (error) {
        std::cerr << "getaddrinfo(\""
                  << node
                  << "\", \""
                  << service
                  << "\") returned "
                  << error
                  << " ("
                  << gai_strerror(error)
                  << ')'
                  << std::endl;
    }
    else {
        for (const struct addrinfo* elem = list;
             elem != NULL;
             elem = elem->ai_next) {
            Address address = *elem;
            std::string cname;

            if (elem->ai_canonname != NULL) {
                cname = elem->ai_canonname;
            }

            result.push_back(Addrinfo(address, cname));
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
        std::cerr << "gethostname("
                  << host
                  << ") returned "
                  << error
                  << std::endl;
    }

    std::string result = host;
    free(host);
    return result;
}

Nameinfo get_nameinfo(const Address& address, int flags)
{
    char* host = static_cast<char*>(std::calloc(sizeof(char), NI_MAXHOST));
    char* serv = static_cast<char*>(std::calloc(sizeof(char), NI_MAXHOST));
    int error = getnameinfo(address.get_addr(), address.get_addrlen(),
                            host, NI_MAXHOST,
                            serv, NI_MAXHOST,
                            flags);

    if (error) {
        std::cerr << "getnameinfo() returned "
                  << error
                  << " ("
                  << gai_strerror(error)
                  << ')'
                  << std::endl;
    }

    Nameinfo result = Nameinfo(host, serv);
    free(serv);
    free(host);
    return result;
}
