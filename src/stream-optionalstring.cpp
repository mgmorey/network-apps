#include "network/optionalstring.h" // OptionalString, operator<<()

#include <ostream>      // std::ostream

std::ostream& Network::operator<<(std::ostream& os,
                                  const OptionalString& string)
{
    if (string.has_value()) {
        os << '"' << std::string(string) << '"';
    }
    else {
        os << "nullptr";
    }

    return os;
}
