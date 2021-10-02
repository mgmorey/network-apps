#include "network-address.h"    // Address, operator<<(), struct
                                // sockaddr, sock_len_type
#include "network-buffer.h"     // Buffer
#include "network-error.h"      // format_error(), get_last_error(),
                                // reset_last_error()

#ifdef _WIN32
#include <winsock2.h>   // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                        // connect(), htons(), inet_ntop()
#else
#include <arpa/inet.h>  // inet_ntop()
#include <sys/socket.h> // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC,
                        // connect(), htons()
#endif

#include <cassert>      // assert()
#include <cstddef>      // offsetof()
#include <iomanip>      // std::hex, std::setfill(), std::setw(),
                        // std::uppercase
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream
#include <string>       // std::string

Network::Address::Address()
{
}

Network::Address::Address(const sockaddr* t_sockaddr, sock_len_type t_socklen) :
    m_value(cbegin(t_sockaddr), cend(t_sockaddr, t_socklen))
{
}

bool Network::Address::operator<(const Address& t_address) const
{
    return (family() < t_address.family() ||
            port() < t_address.port() ||
            text() < t_address.text());
}

bool Network::Address::operator>(const Address& t_address) const
{
    return (family() > t_address.family() ||
            port() > t_address.port() ||
            text() > t_address.text());
}

bool Network::Address::operator==(const Address& t_address) const
{
    return (family() == t_address.family() &&
            port() == t_address.port() &&
            text() == t_address.text());
}

Network::Address::operator Network::Bytes() const
{
    return m_value;
}

Network::Result Network::Address::connect(Fd t_fd, bool t_verbose) const
{
    assert(!empty());

    if (t_verbose) {
        std::cerr << "Trying "
                  << *this
                  << std::endl;
    }

    std::string error;
    auto code {reset_last_error()};
    const auto fd {static_cast<fd_type>(t_fd)};

    if (::connect(fd, addr(), addrlen()) == connect_error) {
        code = get_last_error();
        std::ostringstream oss;
        oss << "Call to connect("
            << fd
            << ", "
            << *this
            << ") failed with error "
            << code
            << ": "
            << format_error(code);
        error = oss.str();
    }

    Result result {code, error};
    assert(result.result() ?
           result.string() != "" :
           result.string() == "");
    return result;
}

bool Network::Address::empty() const
{
    return m_value.empty();
}

Network::Address::family_type Network::Address::family() const
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

Network::Address::port_type Network::Address::port() const
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

std::string Network::Address::text() const
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

const sockaddr* Network::Address::addr() const
{
    return reinterpret_cast<const sockaddr*>(m_value.data());
}

Network::Address::sock_len_type Network::Address::addrlen() const
{
    return static_cast<sock_len_type>(m_value.size());
}

Network::Address::operator const sockaddr&() const
{
    return *reinterpret_cast<const sockaddr*>(m_value.data());
}

const std::byte* Network::Address::cbegin() const
{
    return data();
}

const std::byte* Network::Address::cend() const
{
    return data() + size();
}

const std::byte* Network::Address::data() const
{
    return m_value.data();
}

std::size_t Network::Address::size() const
{
    return m_value.size();
}

Network::Bytes Network::Address::sa_data() const
{
    const auto begin {cbegin() + m_sa_data_offset};
    const auto end {cend() - m_sa_data_offset};

    if (begin <= end) {
        return Bytes(begin, end);
    }
    else {
        return Bytes();
    }
}

Network::Address::family_type Network::Address::sa_family() const
{
    const auto& sa {static_cast<const sockaddr&>(*this)};
    return sa.sa_family;
}

std::string Network::Address::sa_text() const
{
    std::ostringstream oss;
    oss << std::hex;

    for (const auto i : m_value) {
        oss << std::setfill('0')
            << std::setw(2)
            << std::uppercase
            << static_cast<int>(i);
    }

    const auto str {oss.str()};
    std::string result {"0x"};

    if (str.empty()) {
        result += "0";
    }
    else {
        result += str;
    }

    return result;
}

Network::Address::operator const sockaddr_in&() const
{
    return *reinterpret_cast<const sockaddr_in*>(m_value.data());
}

in_addr Network::Address::sin_addr() const
{
    const auto& sin {static_cast<const sockaddr_in&>(*this)};
    return sin.sin_addr;
}

Network::Address::family_type Network::Address::sin_family() const
{
    const auto& sin {static_cast<const sockaddr_in&>(*this)};
    return sin.sin_family;
}

Network::Address::port_type Network::Address::sin_port() const
{
    const auto& sin {static_cast<const sockaddr_in&>(*this)};
    return sin.sin_port;
}

std::string Network::Address::sin_text() const
{
    Buffer buffer {INET_ADDRSTRLEN};
    const auto in {sin_addr()};
    ::inet_ntop(AF_INET, &in, &buffer[0], buffer.size());
    return std::string(buffer);
}

Network::Address::operator const sockaddr_in6&() const
{
    return *reinterpret_cast<const sockaddr_in6*>(m_value.data());
}

in6_addr Network::Address::sin6_addr() const
{
    const auto& sin6 {static_cast<const sockaddr_in6&>(*this)};
    return sin6.sin6_addr;
}

Network::Address::family_type Network::Address::sin6_family() const
{
    const auto& sin6 {static_cast<const sockaddr_in6&>(*this)};
    return sin6.sin6_family;
}

Network::Address::port_type Network::Address::sin6_port() const
{
    const auto& sin6 {static_cast<const sockaddr_in6&>(*this)};
    return sin6.sin6_port;
}

std::string Network::Address::sin6_text() const
{
    Buffer buffer {INET6_ADDRSTRLEN};
    const auto in6 {sin6_addr()};
    ::inet_ntop(AF_INET6, &in6, &buffer[0], buffer.size());
    return std::string(buffer);
}

#ifndef _WIN32

Network::Address::operator const sockaddr_un&() const
{
    return *reinterpret_cast<const sockaddr_un*>(m_value.data());
}

Network::Address::family_type Network::Address::sun_family() const
{
    const auto& sun {static_cast<const sockaddr_un&>(*this)};
    return sun.sun_family;
}

Network::Bytes Network::Address::sun_path() const
{
    const auto begin {cbegin() + m_sun_path_offset};
    const auto end {cend() - m_sun_path_offset};

    if (begin <= end) {
        return Bytes(begin, end);
    }
    else {
        return Bytes();
    }
}

std::string Network::Address::sun_text() const
{
    const auto text {to_string(sun_path())};
    return text.substr(0, text.find('\0'));
}

#endif

const std::byte* Network::Address::cbegin(const sockaddr* t_sockaddr)
{
    return data(t_sockaddr);
}

const std::byte* Network::Address::cend(const sockaddr* t_sockaddr,
                                        sock_len_type t_socklen)
{
    return data(t_sockaddr) + size(t_socklen);
}

const std::byte* Network::Address::data(const sockaddr* t_sockaddr)
{
    return reinterpret_cast<const std::byte*>(t_sockaddr);
}

std::size_t Network::Address::size(sock_len_type t_socklen)
{
    return t_socklen;
}

std::string Network::Address::to_string(const Bytes& t_value)
{
    std::string result(t_value.size(), '\0');

    for (std::string::size_type i = 0; i < t_value.size(); ++i) {
        result[i] = static_cast<std::string::value_type>(t_value[i]);
    }

    return result;
}
