#include "network/address.h"            // Address, in_addr, in6_addr,
                                        // port_type, sockaddr_in,
                                        // sockaddr_in6
#include "network/buffer.h"             // Buffer
#include "network/get-sin-pointer.h"    // get_sin_pointer()
#include "network/get-sin6-pointer.h"   // get_sin6_pointer()

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6
#include <ws2tcpip.h>   // inet_ntop()
#else
#include <arpa/inet.h>  // inet_ntop()
#include <sys/socket.h> // AF_INET, AF_INET6
#endif

#include <string>       // std::string

auto Network::Address::sin() const -> const sockaddr_in*
{
    return get_sin_pointer(m_value);
}

auto Network::Address::sin_addr() const -> in_addr
{
    return sin()->sin_addr;
}

auto Network::Address::sin_port() const -> Network::Address::port_type
{
    return sin()->sin_port;
}

auto Network::Address::sin_text() const -> std::string
{
    Buffer buffer {INET_ADDRSTRLEN};
    const auto in {sin_addr()};
    ::inet_ntop(AF_INET, &in, buffer.data(), buffer.size());
    return std::string(buffer);
}

auto Network::Address::sin6() const -> const sockaddr_in6*
{
    return get_sin6_pointer(m_value);
}

auto Network::Address::sin6_addr() const -> in6_addr
{
    return sin6()->sin6_addr;
}

auto Network::Address::sin6_port() const -> Network::Address::port_type
{
    return sin6()->sin6_port;
}

auto Network::Address::sin6_text() const -> std::string
{
    Buffer buffer {INET6_ADDRSTRLEN};
    const auto in6 {sin6_addr()};
    ::inet_ntop(AF_INET6, &in6, buffer.data(), buffer.size());
    return std::string(buffer);
}
