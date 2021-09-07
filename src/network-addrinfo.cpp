#include "network-addrinfo.h"   // AddrInfo, Hints, Hostname, Result
                                // Service, struct addrinfo,
                                // operator<<()
#include "network-address.h"    // Address, operator<<()
#include "network-family.h"     // Family, operator<<()
#include "network-flags.h"      // Flags, operator<<()
#include "network-format.h"     // Format, operator<<()
#include "network-nullable.h"   // Nullable, operator<<()
#include "network-protocol.h"   // Protocol, operator<<()
#include "network-socktype.h"   // SockType, operator<<()

#ifdef _WIN32
#include <ws2tcpip.h>   // freeaddrinfo(), getaddrinfo()
#else
#include <netdb.h>      // freeaddrinfo(), getaddrinfo()
#endif

#include <iostream>     // std::cerr, std::endl
#include <cassert>      // assert()
#include <sstream>      // std::ostringstream
#include <string>       // std::string

static addrinfo* get_hints(addrinfo* info, const Network::Hints* hints)
{
    if (hints == nullptr) {
        info = nullptr;
    }
    else {
        *info = static_cast<addrinfo>(*hints);
    }

    return info;
}

static addrinfo get_info()
{
    addrinfo info = {0, 0, 0, 0, 0, nullptr, nullptr, nullptr};
    return info;
}

Network::AddrInfo::InputIterator::InputIterator(pointer t_pointer) :
    m_pointer(t_pointer)
{
}

Network::AddrInfo::InputIterator::reference
Network::AddrInfo::InputIterator::operator*() const
{
    return *m_pointer;
}

Network::AddrInfo::InputIterator::pointer
Network::AddrInfo::InputIterator::operator->() const
{
    return m_pointer;
}

Network::AddrInfo::InputIterator&
Network::AddrInfo::InputIterator::operator++()
{
    m_pointer = m_pointer->ai_next;
    return *this;
}

Network::AddrInfo::InputIterator
Network::AddrInfo::InputIterator::operator++(int)
{
    InputIterator it(*this);
    ++(*this);
    return it;
}

Network::AddrInfo::List::List(const Hostname& t_node,
                              const Service& t_service,
                              const Hints* t_hints,
                              bool t_verbose) :
    m_pointer(nullptr)
{
    assert(!t_node.empty() || !t_service.empty());

    if (t_verbose) {
        std::cerr << "Invoking getaddrinfo("
                  << Nullable(t_node)
                  << ", "
                  << Nullable(t_service)
                  << ", "
                  << t_hints
                  << ", ...)"
                  << std::endl
                  << "Using addrinfo hints "
                  << t_hints
                  << ':'
                  << std::endl
                  << *t_hints
                  << std::endl;
    }

    std::string error;
    addrinfo info = get_info();
    addrinfo* hints = get_hints(&info, t_hints);
    const char* node = t_node.empty() ? nullptr : t_node.c_str();
    const char* service = t_service.empty() ? nullptr : t_service.c_str();
    const int code = ::getaddrinfo(node, service, hints, &m_pointer);

    if (code != 0) {
        std::ostringstream oss;
        oss << "getaddrinfo("
            << Nullable(node)
            << ", "
            << Nullable(service)
            << ", "
            << t_hints
            << ") returned "
            << code
            << ": "
            << ::gai_strerror(code);
        error = oss.str();
    }
    else {
        assert(m_pointer != nullptr);
    }

    m_result = Result(code, error);
    assert(m_result.result() ?
           m_result.string() != "" :
           m_result.string() == "");
}

Network::AddrInfo::List::~List()
{
    if (m_pointer != nullptr) {
        ::freeaddrinfo(m_pointer);
    }
}

Network::AddrInfo::InputIterator
Network::AddrInfo::List::begin() const
{
    return m_pointer;
}

Network::AddrInfo::InputIterator
Network::AddrInfo::List::end() const
{
    return nullptr;
}

Network::Result
Network::AddrInfo::List::result() const
{
    return m_result;
}

bool Network::AddrInfo::operator==(const InputIterator& left,
                                   const InputIterator& right)
{
    return left.m_pointer == right.m_pointer;
}

bool Network::AddrInfo::operator!=(const InputIterator& left,
                                   const InputIterator& right)
{
    return left.m_pointer != right.m_pointer;
}
