#include "network-nullable.h"   // Nullable

#include <ostream>      // std::ostream

std::ostream& Network::operator<<(std::ostream& os,
                                  const Nullable& nullable)
{
    if (nullable.null()) {
        os << "nullptr";
    }
    else {
        os << '"' << nullable.m_value << '"';
    }

    return os;
}
