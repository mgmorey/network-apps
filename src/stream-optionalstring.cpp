#include "network/optionalstring.h" // OptionalString, operator<<()

#include <ostream>      // std::ostream

std::ostream& Network::operator<<(std::ostream& os,
                                  const OptionalString& string)
{
    if (string.null()) {
        os << "nullptr";
    }
    else {
        os << '"' << std::string(string) << '"';
    }

    return os;
}
