#include "network-hints.h"      // Hints, operator<<()
#include "network-format.h"     // Format, operator<<()

#include <ostream>      // std::ostream

std::ostream& Network::operator<<(std::ostream& os,
                                  const Hints& hints)
{
    static const std::string delim {", "};
    static const int tab {0};

    os << "Hints("
       << Format("m_flags")
       << hints.m_flags
       << Format(delim, tab, "m_family")
       << hints.m_family
       << Format(delim, tab, "m_socktype")
       << hints.m_socktype
       << Format(delim, tab, "m_protocol")
       << hints.m_protocol
       << ')';
    return os;
}
