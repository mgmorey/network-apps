#include "network.h"

#ifdef _WIN32
#include <winsock2.h>		// closesocket(), connect(),
                                // gethostname(), socket()
#include <ws2tcpip.h>		// NI_MAXHOST, struct addrinfo,
                                // getaddrinfo(), getnameinfo(),
                                // socklen_t
#else
#include <netdb.h>		// NI_MAXHOST, struct addrinfo,
                                // getaddrinfo(), getnameinfo()
#include <sys/socket.h>		// AF_UNSPEC, connect(), socket(),
                                // socklen_t
#include <unistd.h>		// close(), gethostname()
#endif

#include <cassert>		// assert()
#include <cerrno>		// errno
#include <cstring>		// std::memcpy, std::strerror()
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

Address::~Address()
{
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
    assert(size());
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
    assert(size());
    std::string host(NI_MAXHOST, '\0');
    std::string service(NI_MAXHOST, '\0');
    int error = getnameinfo(data(), size(),
                            &host[0], NI_MAXHOST,
                            &service[0], NI_MAXHOST,
                            flags);

    if (error != 0) {
        std::cerr << "getnameinfo() returned "
                  << error
                  << " ("
                  << gai_strerror(error)
                  << ')'
                  << std::endl;
    }

    trim_zeros(host);
    trim_zeros(service);
    return(Endpoint(host, service));
}

std::string Address::service() const
{
    assert(size());
    return endpoint().second;
}

std::string Address::to_hostname() const
{
    assert(size());
    return endpoint().first;
}

std::string Address::to_string() const
{
    assert(size());
    return endpoint(NI_NUMERICHOST).first;
}

void Address::copy(const Address& address)
{
    addr = address.addr;
}

void Address::copy(const struct addrinfo& ai)
{
    assert(ai.ai_addr != NULL);
    assert(ai.ai_addrlen != 0);
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

Socket::~Socket()
{
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

template <class Container>
void copy_addrinfo(Container& dest,
                   const std::string& node,
                   const std::string& service,
                   const struct addrinfo* hints)
{
    struct addrinfo* list = NULL;
    assert(!(node.empty() && service.empty()));
    const char* c_node = node.empty() ? NULL : node.c_str();
    const char* c_service = service.empty() ? NULL : service.c_str();
    int error = getaddrinfo(c_node, c_service, hints, &list);

    if (error != 0) {
        std::cerr << "getaddrinfo(";

        if (!node.empty()) {
            std::cerr << '"'
                      << node
                      << '"';
        }
        else {
            std::cerr << "NULL";
        }

        std::cerr << ", ";

        if (!service.empty()) {
            std::cerr << '"'
                      << service
                      << "\", ";
        }

        std::cerr << "...) returned "
                  << error
                  << " ("
                  << gai_strerror(error)
                  << ')'
                  << std::endl;
    }
    else {
        assert(list != NULL);

        for (const struct addrinfo* elem = list;
             elem != NULL;
             elem = elem->ai_next) {
            dest.push_back(*elem);
        }

        freeaddrinfo(list);
    }
}

Addresses get_addresses(const std::string& node, const struct addrinfo* hints)
{
    Addresses result;
    copy_addrinfo(result, node, "", hints);
    result.unique();
    return result;
}

Addresses get_addresses(const std::string& node, int family, int flags)
{
    Addresses result;
    struct addrinfo hints;
    set_address_hints(&hints, family, flags);
    copy_addrinfo(result, node, "", &hints);
    result.unique();
    return result;
}

Sockets get_sockets(const std::string& node,
                    const std::string& service,
                    const struct addrinfo* hints)
{
    Sockets result;
    copy_addrinfo(result, node, service, hints);
    return result;
}

Hostname get_hostname()
{
    std::string host(NI_MAXHOST, '\0');
    int error = gethostname(&host[0], NI_MAXHOST - 1);

    if (error != 0) {
        std::cerr << "gethostname(...) returned "
                  << error
                  << std::endl;
    }

    trim_zeros(host);
    return host;
}

void set_address_hints(struct addrinfo* ai, int family, int flags)
{
    assert(ai != NULL);
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
    std::memcpy(ai, &hints, sizeof *ai);
}

void trim_zeros(std::string& str)
{
    std::string::size_type pos = str.find('\0');

    if (pos != std::string::npos) {
        str.resize(pos);
    }
}
