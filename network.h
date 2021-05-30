#ifndef NETWORK_H
#define NETWORK_H

#ifdef _WIN32
#include <winsock2.h>		// AF_UNSPEC, gethostname()
#include <ws2tcpip.h>		// struct addrinfo, getaddrinfo()
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
typedef std::pair<class Address, Hostname> Addrinfo;
typedef std::vector<Addrinfo> Addrinfos;

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
    const sockaddr* get_addr() const;
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
    Socket();
    Socket(const Socket& socket);
    Socket(const struct addrinfo& ai);
    Socket& operator=(const Socket& socket);
    Socket& operator=(const struct addrinfo& ai);
    bool operator<(const Socket& socket) const;
    bool operator>(const Socket& socket) const;
    bool operator!=(const Socket& socket) const;
    bool operator==(const Socket& socket) const;
    Address get_addr() const;
    int get_protocol() const;
    int get_socktype() const;

private:
    void copy(const Socket& socket);
    void copy(const struct addrinfo& ai);
    void set_up();

    Address addr;
    int protocol;
    int socktype;
};

Addresses get_addresses(const std::string& host = "",
                        int family = AF_UNSPEC,
                        int flags = 0);
Addrinfos get_addrinfo(const std::string& node,
                       const std::string& service = "",
                       struct addrinfo* hints = NULL);
Hostname get_hostname();
Nameinfo get_nameinfo(const Address& address,
                      int flags = 0);

#endif
