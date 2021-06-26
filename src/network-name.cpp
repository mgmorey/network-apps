#include "network-name.h"       // Name

Network::Name::Name(const std::string &name) :
    null(false),
    value(name)
{
}

Network::Name::Name(const char* name) :
    null(name == NULL),
    value(null ? "" : name)
{
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Name& name)
{
    if (name.null) {
        os << 0;
    }
    else {
        os << '"' << name.value << '"';
    }

    return os;
}
