#ifndef NETWORK_H
#define NETWORK_H

#include <netdb.h>		// struct addrinfo
#include <sys/socket.h>		// PF_UNSPEC

#include <set>			// std::set
#include <string>		// std::string
#include <vector>

typedef std::string Address;
typedef class AddrInfo AddrInfo;
typedef std::set<Address> Addresses;
typedef std::vector<AddrInfo> AddrInfos;
typedef std::string HostName;
typedef std::string NameInfo;

class AddrInfo
{
public:
    AddrInfo();
    AddrInfo(struct addrinfo& new_ai);
    AddrInfo(const AddrInfo& addrInfo);
    ~AddrInfo();
    AddrInfo& operator=(const struct addrinfo& new_ai);
    AddrInfo& operator=(const AddrInfo& addrInfo);
    struct sockaddr* getAddr() const;
    socklen_t getAddrLen() const;

private:
    void cleanup();
    void copy(const struct addrinfo& new_ai);
    void copy(const AddrInfo& addrInfo);
    void init();

    struct addrinfo ai;
};

Addresses getAddresses(int family = PF_UNSPEC,
                       int flags = 0);
Addresses getAddresses(const Address& host,
                       int family = PF_UNSPEC,
                       int flags = 0);
AddrInfos getAddrInfo(const Address& host,
                      struct addrinfo* hints = NULL);
HostName getHostname();
NameInfo getNameInfo(const AddrInfo& addrInfo, int flags);

#endif
