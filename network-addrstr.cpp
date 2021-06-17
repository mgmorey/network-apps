#include "network-addrstr.h"   // get_addrstr()

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo
#else
#include <netdb.h>      // struct addrinfo
#endif

#include <cassert>      // assert()

std::string Network::get_addrstr(const struct addrinfo& other)
{
    std::string result;
    assert(other.ai_addr != NULL);
    assert(other.ai_addrlen != 0);
    const char* data = reinterpret_cast<const char*>(other.ai_addr);
    std::size_t size = other.ai_addrlen;
    result.append(data, size);
    return result;
}
