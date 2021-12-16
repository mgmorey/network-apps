#include "network/address.h"            // Address, sockaddr,
                                        // sockaddr_in, sockaddr_in6,
                                        // sockaddr_un
#include "network/buffer.h"             // Buffer
#include "network/get-sockaddr.h"       // get_sun_path_length()
#include "network/print.h"              // print()
#include "network/to-string.h"          // to_string()

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, AF_UNIX, htons()
#include <ws2tcpip.h>   // inet_ntop()
#else
#include <arpa/inet.h>  // inet_ntop()
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

#ifndef _WIN32

auto Network::Address::sun() const -> const sockaddr_un*
{
    return reinterpret_cast<const sockaddr_un*>(m_value.data());
}

auto Network::Address::sun_family() const -> Network::family_type
{
    return sun()->sun_family;
}

auto Network::Address::sun_length() const -> Network::Address::length_type
{
#ifdef HAVE_SOCKADDR_SA_LEN
    return sun()->sun_len;
#else
    return 0;
#endif
}

auto Network::Address::sun_path() const -> Network::Address::value_type
{
    const auto size {m_value.size()};
    const auto length {
        size < m_sun_path_offset ? 0 :
        get_sun_path_length(sun(), size)
    };
    const auto offset {m_sun_path_offset};
    return m_value.substr(offset, length);
}

auto Network::Address::sun_text() const -> std::string
{
    const auto text {to_string(sun_path())};
    return text.substr(0, text.find('\0'));
}

#endif
