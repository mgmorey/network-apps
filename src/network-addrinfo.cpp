#include "network-addrinfo.h"   // AddrInfo, Hints, Hostname, Result
                                // Service, struct addrinfo,
                                // operator<<()
#include "network-nullable.h"   // Nullable, operator<<()

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
                              const Service& t_service,
                              const Hints* t_hints,
                              bool t_verbose) :
    m_pointer(nullptr)
{
    assert(!t_node.empty() || !t_service.empty());

    if (t_verbose) {
        std::cerr << "Using "
                  << *t_hints
                  << std::endl;
    }

    std::string message;
    addrinfo ai = Network::Hints();
    const auto hints {get_hints(&ai, t_hints)};
    const auto node {t_node.empty() ? nullptr : t_node.c_str()};
    const auto serv {t_service.empty() ? nullptr : t_service.c_str()};

    if (t_verbose) {
        std::cerr << "Calling getaddrinfo("
                  << (node == nullptr ? "<NULL>" : node)
                  << ", "
                  << (serv == nullptr ? "<NULL>" : serv)
                  << ", ...)"
                  << std::endl;
    }

    const auto error = ::getaddrinfo(node, serv, hints, &m_pointer);

    if (error != 0) {
        std::ostringstream oss;
        oss << "Call to getaddrinfo("
            << Nullable(node)
            << ", "
            << Nullable(serv)
            << ", ...) returned "
            << error
            << ": "
            << ::gai_strerror(error);
        message = oss.str();
    }
    else {
        assert(m_pointer != nullptr);
    }

    assert(error == 0 ?
           message == "" :
           message != "");
    m_result = {error, message};
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
