#ifndef NETWORK_H
#define NETWORK_H

#ifdef _WIN32
#include <ws2tcpip.h>		// getaddrinfo(), struct addrinfo
#else
#include <netdb.h>		// struct addrinfo
#include <sys/socket.h>		// PF_UNSPEC
#endif

#include <set>			// std::set
#include <string>		// std::string
#include <vector>		// std::vector
#include <utility>		// std::pair

class Addrinfo
{
public:
    Addrinfo();
    Addrinfo(const struct addrinfo& ai);
    Addrinfo(const Addrinfo& ai);
    ~Addrinfo();
    Addrinfo& operator=(const struct addrinfo& ai);
    Addrinfo& operator=(const Addrinfo& ai);
    const struct sockaddr* get_addr() const;
    socklen_t get_addrlen() const;
    const char* get_canonname() const;
    int get_family() const;
    int get_flags() const;
    int get_protocol() const;
    int get_socktype() const;

private:
    void copy(const struct addrinfo& ai);
    void copy(const Addrinfo& ai);
    void set_up();
    void tear_down();

    struct addrinfo ai;
};

typedef std::pair<int, std::string> Address;
typedef std::set<Address> Addresses;
typedef std::vector<Addrinfo> Addrinfos;
typedef std::string Hostname;
typedef std::string Service;
typedef std::pair<Hostname, Service> Nameinfo;

Addresses get_addresses(const std::string& host = "",
                        int family = PF_UNSPEC,
                        int flags = 0);
Addrinfos get_addrinfo(const std::string& host,
                       struct addrinfo* hints = NULL);
Hostname get_hostname();
Nameinfo get_nameinfo(const Addrinfo& ai, int flags);

#endif
