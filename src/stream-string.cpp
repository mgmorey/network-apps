#include "network-string.h"     // String, operator<<()
#include "network-tostring.h"   // to_string()

#include <ostream>      // std::ostream

std::ostream& Network::operator<<(std::ostream& os,
                                  const ByteString& string)
{
    os << to_string(string, true);
    return os;
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
