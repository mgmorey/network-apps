#include "network-hints.h"      // Hints, operator<<()
#include "network-format.h"     // Format, operator<<()

#include <ostream>      // std::ostream

std::ostream& Network::operator<<(std::ostream& os,
                                  const Hints& hints)
{
    static const std::string delim {", "};
    static const int tab {6};

    os << "Hints("
       << Format("m_flags")
       << hints.flags()
       << Format(delim, tab, "m_family")
       << hints.family()
       << Format(delim, tab, "m_socktype")
       << hints.socktype()
       << Format(delim, tab, "m_protocol")
       << hints.protocol()
       << ')';
    return os;
}
