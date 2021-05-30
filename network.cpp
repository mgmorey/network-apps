#include "network.h"

#ifdef _WIN32
#include <winsock2.h>		// gethostname()
#include <ws2tcpip.h>		// NI_MAXHOST, struct addrinfo,
                                // getaddrinfo(), get_nameinfo(),
                                // socklen_t
#else
#include <netdb.h>		// NI_MAXHOST, struct addrinfo,
                                // getaddrinfo(), get_nameinfo()
#include <sys/socket.h>		// AF_UNSPEC, socklen_t
#include <unistd.h>		// close(), gethostname()
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
            addr < address.addr);
}

bool Address::operator>(const Address& address) const
{
    return (family > address.family ||
            addr > address.addr);
}

bool Address::operator!=(const Address& address) const
{
    return (family != address.family ||
            addr != address.addr);
}

bool Address::operator==(const Address& address) const
{
    return (family == address.family &&
            addr == address.addr);
}

const struct sockaddr* Address::get_addr() const
{
    return reinterpret_cast<const struct sockaddr*>(addr.data());
}

socklen_t Address::get_addrlen() const
{
    return addr.size();
}

int Address::get_family() const
{
    return family;
}

std::string Address::to_hostname() const
{
    return get_nameinfo(*this).first;
}

std::string Address::to_string() const
{
    return get_nameinfo(*this, NI_NUMERICHOST).first;
}

void Address::copy(const Address& address)
{
    addr = address.addr;
    family = address.family;
}

void Address::copy(const struct addrinfo& ai)
{
    addr.clear();
    const char* data = reinterpret_cast<const char*>(ai.ai_addr);
    std::size_t size = ai.ai_addrlen;
    addr.append(data, size);
    family = ai.ai_family;
}

void Address::set_up()
{
    family = 0;
}

Socket::Socket()
{
    set_up();
}

void Socket::close(int fd)
{
#ifdef _WIN32
    closesocket(fd);
#else
    ::close(fd);
#endif
}

Socket::Socket(const Socket& socket)
{
    set_up();
    copy(socket);
}

Socket::Socket(const struct addrinfo& ai)
{
    set_up();
    copy(ai);
}

Socket& Socket::operator=(const Socket& socket)
{
    copy(socket);
    return *this;
}

Socket& Socket::operator=(const struct addrinfo& ai)
{
    copy(ai);
    return *this;
}

bool Socket::operator<(const Socket& socket) const
{
    return (socktype < socket.socktype ||
            protocol < socket.protocol ||
            addr < socket.addr);
}

bool Socket::operator>(const Socket& socket) const
{
    return (socktype > socket.socktype ||
            protocol > socket.protocol ||
            addr > socket.addr);
}

bool Socket::operator!=(const Socket& socket) const
{
    return (socktype != socket.socktype ||
            protocol != socket.protocol ||
            addr != socket.addr);
}

bool Socket::operator==(const Socket& socket) const
{
    return (socktype == socket.socktype &&
            protocol == socket.protocol &&
            addr == socket.addr);
}

int Socket::connect() const
{
    int error = -1;
    int fd = ::socket(addr.get_family(), socktype, protocol);

    if (fd == -1) {
        std::cerr << "socket("
                  << addr.get_family()
                  << ", "
                  << socktype
                  << ", "
                  << protocol
                  << ") returned "
                  << fd
                  << std::endl;
        goto clean_up;
    }

    error = ::connect(fd, addr.get_addr(), addr.get_addrlen());

    if (error == -1) {
        std::cerr << "connect("
                  << fd
                  << ") returned "
                  << error
                  << ": "
                  << strerror(errno)
                  << std::endl;
        goto clean_up_socket;
    }

    return fd;

clean_up_socket:
    close(fd);

clean_up:
    return -1;
}

Address Socket::get_addr() const
{
    return addr;
}

std::string Socket::get_cname() const
{
    return cname;
}

int Socket::get_protocol() const
{
    return protocol;
}

int Socket::get_socktype() const
{
    return socktype;
}

void Socket::copy(const Socket& socket)
{
    addr = socket.addr;
    cname = socket.cname;
    protocol = socket.protocol;
    socktype = socket.socktype;
}

void Socket::copy(const struct addrinfo& ai)
{
    addr = ai;

    if (ai.ai_canonname != NULL) {
        cname = ai.ai_canonname;
    }

    protocol = ai.ai_protocol;
    socktype = ai.ai_socktype;
}

void Socket::set_up()
{
    protocol = 0;
    socktype = 0;
}

Addresses get_addresses(const std::string& node, int family, int flags)
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
    Sockets sockets = get_sockets(node, "", &hints);
    Addresses result;

    for (Sockets::const_iterator it = sockets.begin();
         it != sockets.end();
         ++it) {
        result.insert(it->get_addr());
    }

    return result;
}

Sockets get_sockets(const std::string& node,
                    const std::string& service,
                    struct addrinfo* hints)
{
    struct addrinfo* list = NULL;
    int error = getaddrinfo(node.empty() ? NULL : node.c_str(),
                            service.empty() ? NULL : service.c_str(),
                            hints,
                            &list);
    Sockets result;

    if (error != 0) {
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
            result.push_back(*elem);
        }

        freeaddrinfo(list);
    }

    return result;
}

Hostname get_hostname()
{
    char* host = static_cast<char*>(std::calloc(sizeof(char), NI_MAXHOST));
    int error = gethostname(host, NI_MAXHOST - 1);

    if (error != 0) {
        std::cerr << "gethostname("
                  << host
                  << ") returned "
                  << error
                  << std::endl;
    }

    std::string result(host);
    free(host);
    return result;
}

Nameinfo get_nameinfo(const struct sockaddr* addr, socklen_t addrlen, int flags)
{
    char* host = static_cast<char*>(std::calloc(sizeof(char), NI_MAXHOST));
    char* serv = static_cast<char*>(std::calloc(sizeof(char), NI_MAXHOST));
    int error = getnameinfo(addr, addrlen,
                            host, NI_MAXHOST,
                            serv, NI_MAXHOST,
                            flags);

    if (error != 0) {
        std::cerr << "getnameinfo() returned "
                  << error
                  << " ("
                  << gai_strerror(error)
                  << ')'
                  << std::endl;
    }

    Nameinfo result(host, serv);
    free(serv);
    free(host);
    return result;
}

Nameinfo get_nameinfo(const Address& address, int flags)
{
    return get_nameinfo(address.get_addr(), address.get_addrlen(), flags);
}
