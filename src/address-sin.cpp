#include "network/address.h"            // AF_INET, INET6_ADDRSTRLEN,
                                        // Address, in_addr
                                        // inet_ntop(), port_type,
                                        // sockaddr_in
#include "network/buffer.h"             // Buffer
#include "network/get-sin-pointer.h"    // get_sin_pointer()

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
