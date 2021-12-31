#include "network/address.h"            // AF_INET6, INET6_ADDRSTRLEN,
                                        // Address, in6_addr
                                        // inet_ntop(), port_type,
                                        // sockaddr_in6
#include "network/buffer.h"             // Buffer
#include "network/get-sin6-pointer.h"   // get_sin6_pointer()

#include <string>       // std::string

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
