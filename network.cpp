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
#include <cstdlib>		// free()
#include <cstring>		// std::memcpy(), strdup(),
                                // std::strerror()
#include <iostream>		// std::cerr, std::endl

void close_socket(int fd)
{
    if (fd == -1) {
        return;
    }

#ifdef _WIN32
    ::closesocket(fd);
#else
    ::close(fd);
#endif
}

ConnectResult connect_socket(const std::string& host,
                             const std::string& service,
                             const struct addrinfo &hints)
{
    Sockets sockets(get_sockets(host, service, hints));
    std::string canonical_name;
    int fd = -1;

    for (Sockets::const_iterator it = sockets.begin();
         it != sockets.end();
         ++it) {
        fd = it->socket();

        if (fd == -1) {
            continue;
        }

        if (it->connect(fd) == 0) {
            canonical_name = it->cname();
            break;
        }
    }

    return ConnectResult(fd, canonical_name);
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

Addresses get_addresses(const std::string& node,
                        const struct addrinfo* hints)
{
    Addresses result;
    copy_addrinfo(result, node, "", hints);
    result.unique();
    return result;
}

Addresses get_addresses(const std::string& node,
                        const struct addrinfo& hints)
{
    return get_addresses(node, &hints);
}

Addresses get_addresses(const std::string& node,
                        int family, int flags)
{
    return get_addresses(node, Socket(family, flags));
}

Sockets get_sockets(const std::string& node,
                    const std::string& service,
                    const struct addrinfo* hints)
{
    Sockets result;
    copy_addrinfo(result, node, service, hints);
    return result;
}

Sockets get_sockets(const std::string& node,
                    const std::string& service,
                    const struct addrinfo& hints)
{
    return get_sockets(node, service, &hints);
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
