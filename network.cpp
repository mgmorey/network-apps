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
#include <cstdio>		// free()
#include <cstring>		// std::memcpy(), strdup(),
                                // std::strerror()
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

Address::Address(const Address& other)
{
    copy(other);
}

Address::Address(const struct addrinfo& other)
{
    copy(other);
}

Address::~Address()
{
}

Address& Address::operator=(const Address& other)
{
    copy(other);
    return *this;
}

Address& Address::operator=(const struct addrinfo& other)
{
    copy(other);
    return *this;
}

bool Address::operator<(const Address& other) const
{
    return (addr < other.addr);
}

bool Address::operator>(const Address& other) const
{
    return (addr > other.addr);
}

bool Address::operator!=(const Address& other) const
{
    return (addr != other.addr);
}

bool Address::operator==(const Address& other) const
{
    return (addr == other.addr);
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

void Address::copy(const Address& other)
{
    addr = other.addr;
}

void Address::copy(const struct addrinfo& other)
{
    assert(other.ai_addr != NULL);
    assert(other.ai_addrlen != 0);
    addr.clear();
    const char* data = reinterpret_cast<const char*>(other.ai_addr);
    std::size_t size = other.ai_addrlen;
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

Socket::Socket(const Socket& other)
{
    set_up();
    copy(other);
}

Socket::Socket(const struct addrinfo& other)
{
    set_up();
    copy(other);
}

Socket::Socket(int protocol, int socktype, int family, int flags)
{
    set_up(protocol, socktype, family, flags);
}

Socket::~Socket()
{
    free(ai.ai_addr);
    free(ai.ai_canonname);
}

Socket& Socket::operator=(const Socket& other)
{
    copy(other);
    return *this;
}

Socket& Socket::operator=(const struct addrinfo& other)
{
    copy(other);
    return *this;
}

bool Socket::operator<(const Socket& other) const
{
    return (ai.ai_protocol < other.ai.ai_protocol ||
            ai.ai_socktype < other.ai.ai_socktype ||
            ai.ai_family < other.ai.ai_family ||
            Address(*this) < Address(other));
}

bool Socket::operator>(const Socket& other) const
{
    return (ai.ai_protocol > other.ai.ai_protocol ||
            ai.ai_socktype > other.ai.ai_socktype ||
            ai.ai_family > other.ai.ai_family ||
            Address(*this) > Address(other));
}

bool Socket::operator!=(const Socket& other) const
{
    return (ai.ai_protocol != other.ai.ai_protocol ||
            ai.ai_socktype != other.ai.ai_socktype ||
            ai.ai_family != other.ai.ai_family ||
            Address(*this) != Address(other));
}

bool Socket::operator==(const Socket& other) const
{
    return (ai.ai_protocol == other.ai.ai_protocol &&
            ai.ai_socktype == other.ai.ai_socktype &&
            ai.ai_family == other.ai.ai_family &&
            Address(*this) == Address(other));
}

Socket::operator const struct addrinfo&() const
{
    return ai;
}

std::string Socket::cname() const
{
    std::string result;

    if (ai.ai_canonname != NULL) {
        result = ai.ai_canonname;
    }

    return result;
}

int Socket::connect(int fd) const
{
    Address address(*this);
    int error = -1;

    if (fd == -1) {
        fd = this->socket();

        if (fd == -1) {
            goto clean_up;
        }
    }

    error = address.connect(fd);

    if (error == -1) {
        goto clean_up_socket;
    }

    return fd;

clean_up_socket:
    Address::close(fd);

clean_up:
    return -1;
}

int Socket::socket() const
{
    int fd = ::socket(ai.ai_family, ai.ai_socktype, ai.ai_protocol);

    if (fd == -1) {
        std::cerr << "socket("
                  << ai.ai_family
                  << ", "
                  << ai.ai_socktype
                  << ", "
                  << ai.ai_protocol
                  << ") returned "
                  << fd
                  << std::endl;
    }

    return fd;
}

void Socket::copy(const Socket& other)
{
    copy(other.ai);
}

void Socket::copy(const struct addrinfo& other)
{
    copy_addrinfo(ai, other);
}

void Socket::set_up(int protocol, int socktype, int family, int flags)
{
    init_addrinfo(ai, protocol, socktype, family, flags);
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

void copy_addrinfo(struct addrinfo& dest, const struct addrinfo& src)
{
    dest.ai_flags = src.ai_flags;
    dest.ai_family = src.ai_family;
    dest.ai_socktype = src.ai_socktype;
    dest.ai_protocol = src.ai_protocol;
    dest.ai_addrlen = src.ai_addrlen;
    free(dest.ai_canonname);
    dest.ai_canonname = NULL;

    if (src.ai_canonname != NULL) {
        dest.ai_canonname = strdup(src.ai_canonname);
    }

    free(dest.ai_addr);
    dest.ai_addr = NULL;

    if (src.ai_addr != NULL) {
        dest.ai_addr = static_cast<struct sockaddr*>(malloc(dest.ai_addrlen));
        std::memcpy(dest.ai_addr, src.ai_addr, dest.ai_addrlen);
    }

    dest.ai_next = NULL;
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
    Socket socket(0, 0, family, flags);
    const struct addrinfo& hints = socket;
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

void init_addrinfo(struct addrinfo& dest,
                   int protocol,
                   int socktype,
                   int family,
                   int flags)
{
    std::memset(&dest, '\0', sizeof dest);
    dest.ai_flags = flags;
    dest.ai_family = family;
    dest.ai_socktype = socktype;
    dest.ai_protocol = protocol;
    dest.ai_addrlen = 0;
    dest.ai_canonname = NULL;
    dest.ai_addr = NULL;
    dest.ai_next = NULL;
}

void trim_zeros(std::string& str)
{
    std::string::size_type pos = str.find('\0');

    if (pos != std::string::npos) {
        str.resize(pos);
    }
}
