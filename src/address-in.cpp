#include "network/address.h"            // Address, sockaddr,
                                        // sockaddr_in, sockaddr_in6
#include "network/buffer.h"             // Buffer

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
    return reinterpret_cast<const sockaddr_in*>(m_value.data());
}

auto Network::Address::sin_addr() const -> in_addr
{
    return sin()->sin_addr;
}

auto Network::Address::sin_family() const -> Network::family_type
{
    return sin()->sin_family;
}

auto Network::Address::sin_length() const -> Network::Address::length_type
{
#ifdef HAVE_SOCKADDR_SA_LEN
    return sin()->sin_len;
#else
    return 0;
#endif
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
    return reinterpret_cast<const sockaddr_in6*>(m_value.data());
}

auto Network::Address::sin6_addr() const -> in6_addr
{
    return sin6()->sin6_addr;
}

auto Network::Address::sin6_family() const -> Network::family_type
{
    return sin6()->sin6_family;
}

auto Network::Address::sin6_length() const -> Network::Address::length_type
{
#ifdef HAVE_SOCKADDR_SA_LEN
    return sin6()->sin6_len;
#else
    return 0;
#endif
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
