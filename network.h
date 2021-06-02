#ifndef NETWORK_H
#define NETWORK_H

#ifdef _WIN32
#include <winsock2.h>		// AF_UNSPEC
#include <ws2tcpip.h>		// struct addrinfo, socklen_t
#else
#include <netdb.h>		// struct addrinfo
#include <sys/socket.h>		// AF_UNSPEC, socklen_t
#endif

#include <list>			// std::list
#include <string>		// std::string
#include <utility>		// std::pair

typedef std::string Hostname;
typedef std::string Service;

typedef std::pair<Hostname, Service> Endpoint;
typedef std::list<class Address> Addresses;
typedef std::list<class Socket> Sockets;

class Address
{
public:
    static void close(int fd);

    Address();
    Address(const Address& other);
    Address(const struct addrinfo& other);
    ~Address();
    Address& operator=(const Address& other);
    Address& operator=(const struct addrinfo& other);
    bool operator<(const Address& other) const;
    bool operator>(const Address& other) const;
    bool operator!=(const Address& other) const;
    bool operator==(const Address& other) const;
    int connect(int fd) const;
    Endpoint endpoint(int flags = 0) const;
    std::string service() const;
    std::string to_hostname() const;
    std::string to_string() const;

private:
    void copy(const Address& address);
    void copy(const struct addrinfo& ai);
    const struct sockaddr* data() const;
    socklen_t size() const;

    std::string addr;
};

class Socket
{
public:
    Socket();
    Socket(const Socket& other);
    Socket(const struct addrinfo& other);
    Socket(int protocol, int socktype, int family, int flags = 0);
    Socket(int family, int flags = 0);
    ~Socket();
    Socket& operator=(const Socket& other);
    Socket& operator=(const struct addrinfo& ai);
    bool operator<(const Socket& other) const;
    bool operator>(const Socket& other) const;
    bool operator!=(const Socket& other) const;
    bool operator==(const Socket& other) const;
    operator const struct addrinfo&() const;
    std::string cname() const;
    int connect(int fd = -1) const;
    int socket() const;

private:
    void copy(const Socket& other);
    void copy(const struct addrinfo& other);
    void init(int protocol = 0,
              int socktype = 0,
              int family = 0,
              int flags = 0);

    struct addrinfo ai;
};

template <class Container>
extern void copy_addrinfo(Container& dest,
                          const std::string& node,
                          const std::string& service,
                          const struct addrinfo* hints);
extern void copy_addrinfo(struct addrinfo& dest,
                          const struct addrinfo& src);
extern Addresses get_addresses(const std::string& node,
                               const struct addrinfo* hints = NULL);
extern Addresses get_addresses(const std::string& node,
                               const struct addrinfo& hints);
extern Addresses get_addresses(const std::string& node,
                               int family, int flags = 0);
extern Sockets get_sockets(const std::string& node,
                           const std::string& service = "",
                           const struct addrinfo* hints = NULL);
extern Hostname get_hostname();
extern void init_addrinfo(struct addrinfo& dest,
                          int protocol = 0,
                          int socktype = 0,
                          int family = 0,
                          int flags = 0);
extern void trim_zeros(std::string& str);

#endif
