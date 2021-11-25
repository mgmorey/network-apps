#include "network/format.h"     // Format, std::ostream, std::string

#include <utility>      // std::move()

Network::Format::Format(std::string t_delimiter, int t_indent,
                        std::string t_key) :
    m_delimiter(std::move(t_delimiter)),
    m_indent(t_indent),
    m_key(std::move(t_key))
{
}

std::ostream& Network::operator<<(std::ostream& os,
                                  const Format& format)
{
    if (format.m_indent >= 0) {
        os << format.m_delimiter;

        if (format.m_indent > 0) {
            os << std::endl
               << std::string(format.m_indent, ' ');
        }
    }

    if (!format.m_key.empty()) {
        os << format.m_key
           << '=';
    }

    return os;
}
