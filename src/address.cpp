#include "network/address.h"            // Address, sockaddr,
#include "network/print.h"              // print()

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, AF_UNIX, htons()
#else
#include <sys/socket.h> // AF_INET, AF_INET6, AF_UNIX, htons()
#endif

#include <sstream>      // std::ostringstream
#include <string>       // std::string
#include <utility>      // std::move

Network::Address::Address(value_type t_value) :
    m_value(std::move(t_value))
{
}

auto Network::Address::operator=(const value_type& t_value) -> Network::Address&
{
    m_value = t_value;
    return *this;
}

auto Network::Address::operator<(const Address& t_address) const -> bool
{
    return (family() < t_address.family() ||
            port() < t_address.port() ||
            text() < t_address.text());
}

auto Network::Address::operator>(const Address& t_address) const -> bool
{
    return (family() > t_address.family() ||
            port() > t_address.port() ||
            text() > t_address.text());
}

auto Network::Address::operator==(const Address& t_address) const -> bool
{
    return (family() == t_address.family() &&
            port() == t_address.port() &&
            text() == t_address.text());
}

Network::Address::operator value_type() const
{
    return m_value;
}

auto Network::Address::empty() const -> bool
{
    return m_value.empty();
}

auto Network::Address::family() const -> Network::family_type
{
    switch (sa_family()) {
#ifndef _WIN32
    case AF_UNIX:
        return sun_family();
#endif
    case AF_INET:
        return sin_family();
    case AF_INET6:
        return sin6_family();
    default:
        return sa_family();
    }
}

auto Network::Address::length() const -> Network::Address::length_type
{
    switch (sa_family()) {
#ifndef _WIN32
    case AF_UNIX:
        return sun_length();
#endif
    case AF_INET:
        return sin_length();
    case AF_INET6:
        return sin6_length();
    default:
        return sa_length();
    }
}

auto Network::Address::port() const -> Network::Address::port_type
{
    switch (sa_family()) {
    case AF_INET:
        return htons(sin_port());
    case AF_INET6:
        return htons(sin6_port());
    default:
        return 0;
    }
}

auto Network::Address::text() const -> std::string
{
    switch (sa_family()) {
#ifndef _WIN32
    case AF_UNIX:
        return sun_text();
#endif
    case AF_INET:
        return sin_text();
    case AF_INET6:
        return sin6_text();
    default:
        return sa_text();
    }
}

auto Network::Address::sa() const -> const sockaddr*
{
    return reinterpret_cast<const sockaddr*>(m_value.data());
}

auto Network::Address::sa_data() const -> Network::Address::value_type
{
    return m_value.substr(m_sa_data_offset);
}

auto Network::Address::sa_family() const -> Network::family_type
{
    return sa()->sa_family;
}

auto Network::Address::sa_length() const -> Network::Address::length_type
{
#ifdef HAVE_SOCKADDR_SA_LEN
    return sa()->sa_len;
#else
    return 0;
#endif
}

auto Network::Address::sa_text() const -> std::string
{
    std::ostringstream oss;
    print(oss, m_value);
    return oss.str();
}
