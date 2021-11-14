#include "network/addrinfo.h"   // AddrInfo, Hints, Hostname, Result
                                // Service, addrinfo, operator<<()

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
        *info = *hints;
    }

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
                              const Service& t_serv,
                              const Hints* t_hints,
                              bool t_verbose)
{
    assert(m_pointer == nullptr);

    if (t_verbose) {
        std::cerr << "Trying socket hints:"
                  << std::endl
                  << *t_hints
                  << std::endl;
    }

    addrinfo ai = Network::Hints();
    const auto hints {get_hints(&ai, t_hints)};

    if (t_verbose) {
        std::cerr << "Calling getaddrinfo("
                  << t_node
                  << ", "
                  << t_serv
                  << ", ...)"
                  << std::endl;
    }

    if (const auto error = ::getaddrinfo(t_node, t_serv, hints, &m_pointer)) {
        std::ostringstream oss;
        oss << "Call to getaddrinfo("
            << t_node
            << ", "
            << t_serv
            << ", ...) returned "
            << error
            << ": "
            << ::gai_strerror(error);
        m_result = {error, oss.str()};
    }
    else {
        assert(m_pointer != nullptr);
    }
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
