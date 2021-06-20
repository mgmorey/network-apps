#include "network-format.h"     // Format

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
    if (format.ind > 0) {
        os << format.del << std::endl;

        if (format.ind > 0) {
            os << std::string(format.ind, ' ');
        }
    }
    else {
        os << format.del;
    }

    if (!format.str.empty()) {
        os << format.str << '=';
    }

    return os;
}

const std::string Network::Format::del_default = ",";
const int Network::Format::ind_default = -1;
