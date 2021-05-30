#ifndef NETWORK_H
#define NETWORK_H

#ifdef _WIN32
#include <winsock2.h>		// AF_UNSPEC
#include <ws2tcpip.h>		// struct addrinfo, socklen_t
#else
#include <netdb.h>		// struct addrinfo
#include <sys/socket.h>		// AF_UNSPEC, socklen_t
#endif

#include <set>			// std::set
#include <string>		// std::string
#include <vector>		// std::vector
#include <utility>		// std::pair

typedef std::string Hostname;
typedef std::string Service;
typedef std::pair<Hostname, Service> Nameinfo;
typedef std::set<class Address> Addresses;
typedef std::vector<class Socket> Sockets;

class Address
{
public:
    Address();
    Address(const Address& address);
    Address(const struct addrinfo& ai);
    Address& operator=(const Address& address);
    Address& operator=(const struct addrinfo& ai);
    bool operator<(const Address& address) const;
    bool operator>(const Address& address) const;
    bool operator!=(const Address& address) const;
    bool operator==(const Address& address) const;
    const struct sockaddr* get_addr() const;
    socklen_t get_addrlen() const;
    int get_family() const;
    std::string to_hostname() const;
    std::string to_string() const;

private:
    void copy(const Address& address);
    void copy(const struct addrinfo& ai);
    void set_up();

    std::string addr;
    int family;
};

class Socket
{
public:
    static void close(int fd);

    Socket();
    Socket(const Socket& socket);
    Socket(const struct addrinfo& ai);
    Socket& operator=(const Socket& socket);
    Socket& operator=(const struct addrinfo& ai);
    bool operator<(const Socket& socket) const;
    bool operator>(const Socket& socket) const;
    bool operator!=(const Socket& socket) const;
    bool operator==(const Socket& socket) const;
    int connect() const;
    Address get_addr() const;
    std::string get_cname() const;
    int get_protocol() const;
    int get_socktype() const;

private:
    void copy(const Socket& socket);
    void copy(const struct addrinfo& ai);
    void set_up();

    Address addr;
    std::string cname;
    int protocol;
    int socktype;
};

Addresses get_addresses(const std::string& node = "",
                        int family = AF_UNSPEC,
                        int flags = 0);
Sockets get_sockets(const std::string& node,
                    const std::string& service = "",
                    struct addrinfo* hints = NULL);
Hostname get_hostname();
Nameinfo get_nameinfo(const struct sockaddr* addr,
                      socklen_t addrlen,
                      int flags = 0);
Nameinfo get_nameinfo(const Address& address,
                      int flags = 0);

#endif
