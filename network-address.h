#ifndef NETWORK_ADDRESS_H
#define NETWORK_ADDRESS_H

#include "network-types.h"

#ifdef _WIN32
#include <ws2tcpip.h>		// struct addrinfo, socklen_t
#else
#include <netdb.h>		// struct addrinfo
#include <sys/socket.h>		// socklen_t
#endif

#include <string>		// std::string
#include <utility>		// std::pair

typedef std::pair<Hostname, Service> Endpoint;

class Address
{
public:
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
    Service service() const;
    Hostname to_hostname() const;
    std::string to_string() const;

private:
    void copy(const Address& other);
    void copy(const struct addrinfo& other);
    const struct sockaddr* data() const;
    socklen_t size() const;

    std::string addr;
};

#endif
