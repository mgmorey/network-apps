#include "network/print.h"      // print()
#include "network/string.h"     // String, operator<<()

#include <ostream>      // std::ostream

std::ostream& Network::operator<<(std::ostream& os,
                                  const ByteString& string)
{
    return print(os, string);
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const String& string)
{
    if (string.null()) {
        os << "nullptr";
    }
    else {
        os << '"' << string.m_value << '"';
    }

    return os;
}
