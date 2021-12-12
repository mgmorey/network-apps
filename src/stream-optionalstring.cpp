#include "network/optionalstring.h" // OptionalString, operator<<()

#include <ostream>      // std::ostream

auto Network::operator<<(std::ostream& os,
                         const OptionalString& string) -> std::ostream&
{
    if (string.has_value()) {
        os << '"' << string.value() << '"';
    }
    else {
        os << "nullptr";
    }

    return os;
}
