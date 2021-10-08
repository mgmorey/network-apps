#include "network-optional.h"   // Optional

#include <ostream>      // std::ostream

std::ostream& Network::operator<<(std::ostream& os,
                                  const Optional& optional)
{
    if (optional.null()) {
        os << "nullptr";
    }
    else {
        os << '"' << optional.m_value << '"';
    }

    return os;
}
