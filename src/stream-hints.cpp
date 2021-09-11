#include "network-hints.h"      // Hints, operator<<()
#include "network-format.h"     // Format, operator<<()

#include <ostream>      // std::ostream

std::ostream& Network::operator<<(std::ostream& os,
                                  const Hints& hints)
{
    static const int tabs[1] = {6};

    os << "Hints("
       << Format("m_flags")
       << hints.m_flags
       << Format(tabs[0], "m_family")
       << hints.m_family
       << Format(tabs[0], "m_socktype")
       << hints.m_socktype
       << Format(tabs[0], "m_protocol")
       << hints.m_protocol
       << ')';
    return os;
}
