#include "network.h"

#ifdef _WIN32
#include <winsock2.h>		// gethostname(), closesocket()
#include <ws2tcpip.h>		// NI_MAXHOST, struct addrinfo,
                                // getaddrinfo(), getnameinfo(),
                                // socklen_t
#else
#include <netdb.h>		// NI_MAXHOST, struct addrinfo,
                                // getaddrinfo(), getnameinfo()
#include <sys/socket.h>		// AF_UNSPEC, socklen_t
#include <unistd.h>		// close(), gethostname()
#endif

#include <cerrno>		// errno
#include <cstdlib>		// std::calloc()
#include <cstring>		// std::strerror()
#include <iostream>		// std::cerr, std::endl

void Address::close(int fd)
{
#ifdef _WIN32
    closesocket(fd);
#else
    ::close(fd);
#endif
}

Address::Address()
{
}

Address::Address(const Address& address)
{
    copy(address);
}

Address::Address(const struct addrinfo& ai)
{
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
    return (addr < address.addr);
}

bool Address::operator>(const Address& address) const
{
    return (addr > address.addr);
}

bool Address::operator!=(const Address& address) const
{
    return (addr != address.addr);
}

bool Address::operator==(const Address& address) const
{
    return (addr == address.addr);
}

int Address::connect(int fd) const
{
    assert(!addr.empty());
    int error = -1;

    if (fd == -1) {
        goto clean_up;
    }

    error = ::connect(fd, data(), size());

    if (error == -1) {
        std::cerr << "connect("
                  << fd
                  << ") returned "
                  << error
                  << ": "
                  << std::strerror(errno)
                  << std::endl;
        goto clean_up_socket;
    }

    return fd;

clean_up_socket:
    Address::close(fd);

clean_up:
    return -1;
}

Endpoint Address::endpoint(int flags) const
{
    assert(!addr.empty());
    char* host = static_cast<char*>(std::calloc(sizeof(char), NI_MAXHOST));
    char* serv = static_cast<char*>(std::calloc(sizeof(char), NI_MAXHOST));
    int error = getnameinfo(data(), size(),
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

    Endpoint result(host, serv);
    free(serv);
    free(host);
    return result;
}

std::string Address::service() const
{
    assert(!addr.empty());
    return endpoint().second;
}

std::string Address::to_hostname() const
{
    assert(!addr.empty());
    return endpoint().first;
}

std::string Address::to_string() const
{
    assert(!addr.empty());
    return endpoint(NI_NUMERICHOST).first;
}

void Address::copy(const Address& address)
{
    addr = address.addr;
}

void Address::copy(const struct addrinfo& ai)
{
    addr.clear();
    const char* data = reinterpret_cast<const char*>(ai.ai_addr);
    std::size_t size = ai.ai_addrlen;
    addr.append(data, size);
}

const struct sockaddr* Address::data() const
{
    return reinterpret_cast<const struct sockaddr*>(addr.data());
}

socklen_t Address::size() const
{
    return static_cast<socklen_t>(addr.size());
}

Socket::Socket()
{
    set_up();
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
            family < socket.family ||
            addr < socket.addr);
}

bool Socket::operator>(const Socket& socket) const
{
    return (socktype > socket.socktype ||
            protocol > socket.protocol ||
            family > socket.family ||
            addr > socket.addr);
}

bool Socket::operator!=(const Socket& socket) const
{
    return (socktype != socket.socktype ||
            protocol != socket.protocol ||
            family != socket.family ||
            addr != socket.addr);
}

bool Socket::operator==(const Socket& socket) const
{
    return (socktype == socket.socktype &&
            protocol == socket.protocol &&
            family == socket.family &&
            addr == socket.addr);
}

int Socket::connect(int fd) const
{
    int error = -1;

    if (fd == -1) {
        fd = socket();

        if (fd == -1) {
            goto clean_up;
        }
    }

    error = addr.connect(fd);

    if (error == -1) {
        goto clean_up_socket;
    }

    return fd;

clean_up_socket:
    Address::close(fd);

clean_up:
    return -1;
}

std::string Socket::canonical_hostname() const
{
    return cname;
}

int Socket::socket() const
{
    int fd = ::socket(family, socktype, protocol);

    if (fd == -1) {
        std::cerr << "socket("
                  << family
                  << ", "
                  << socktype
                  << ", "
                  << protocol
                  << ") returned "
                  << fd
                  << std::endl;
    }

    return fd;
}

void Socket::copy(const Socket& socket)
{
    addr = socket.addr;
    cname = socket.cname;
    family = socket.family;
    protocol = socket.protocol;
    socktype = socket.socktype;
}

void Socket::copy(const struct addrinfo& ai)
{
    addr = ai;

    if (ai.ai_canonname == NULL) {
        cname.clear();
    }
    else {
        cname = ai.ai_canonname;
    }

    family = ai.ai_family;
    protocol = ai.ai_protocol;
    socktype = ai.ai_socktype;
}

void Socket::set_up()
{
    family = 0;
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
    struct addrinfo* list = NULL;
    int error = getaddrinfo(node.empty() ? NULL : node.c_str(),
                            NULL,
                            &hints,
                            &list);
    Addresses result;

    if (error != 0) {
        std::cerr << "getaddrinfo(\""
                  << node
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
            result.insert(*elem);
        }

        freeaddrinfo(list);
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
