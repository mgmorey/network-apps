#include "network/hints.h"      // Hints, operator<<()
#include "network/format.h"     // Format, operator<<()

#include <ostream>      // std::ostream

auto Network::operator<<(std::ostream& os,
                         const Hints& hints) -> std::ostream&
{
    static constexpr auto delim {", "};
    static constexpr auto tab {6};

    os << "Hints("
       << Format("flags")
       << hints.flags()
       << Format(delim, tab, "family")
       << hints.family()
       << Format(delim, tab, "socktype")
       << hints.socktype()
       << Format(delim, tab, "protocol")
       << hints.protocol()
       << ')';
    return os;
}
