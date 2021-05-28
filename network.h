#ifndef NETWORK_H
#define NETWORK_H

#include <netdb.h>		// struct addrinfo
#include <sys/socket.h>		// PF_UNSPEC

#include <set>			// std::set
#include <string>		// std::string
#include <vector>

class Addrinfo
{
public:
    Addrinfo();
    Addrinfo(struct addrinfo& new_ai);
    Addrinfo(const Addrinfo& addrInfo);
    ~Addrinfo();
    Addrinfo& operator=(const struct addrinfo& new_ai);
    Addrinfo& operator=(const Addrinfo& addrInfo);
    const struct sockaddr* get_addr() const;
    socklen_t get_addrlen() const;
    const char* get_canonname() const;
    int get_family() const;
    int get_flags() const;
    int get_protocol() const;
    int get_socktype() const;

private:
    void copy(const struct addrinfo& new_ai);
    void copy(const Addrinfo& addrInfo);
    void reset();
    void set_up();

    struct addrinfo ai;
};

typedef std::string Address;
typedef std::set<Address> Addresses;
typedef std::vector<Addrinfo> Addrinfos;
typedef std::string Hostname;
typedef std::string Service;
typedef std::pair<Hostname, Service> Nameinfo;

Addresses get_addresses(int family = PF_UNSPEC,
                        int flags = 0);
Addresses get_addresses(const Address& host,
                        int family = PF_UNSPEC,
                        int flags = 0);
Addrinfos get_addrinfo(const Address& host,
                       struct addrinfo* hints = NULL);
Hostname get_hostname();
Nameinfo get_nameinfo(const Addrinfo& addrInfo, int flags);

#endif
