#include "network-format.h"     // Format, std::ostream, std::string

Network::Format::Format(const std::string& key) :
    str(key),
    del(del_default),
    ind(ind_default)
{
}

Network::Format::Format(int indent, const std::string& key) :
    str(key),
    del(del_default),
    ind(indent)
{
}

Network::Format::Format(const std::string& delim, int indent,
                        const std::string& key) :
    str(key),
    del(delim),
    ind(indent)
{
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Format& format)
{
    if (format.ind >= 0) {
        os << format.del;

        if (format.ind > 0) {
            os << std::endl << std::string(format.ind, ' ');
        }
    }

    if (!format.str.empty()) {
        os << format.str << '=';
    }

    return os;
}

const std::string Network::Format::del_default = ",";
const int Network::Format::ind_default = -1;
