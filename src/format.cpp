#include "network/format.h"     // Format, std::ostream, std::string

Network::Format::Format(const std::string& t_key) :
    m_value(t_key)
{
}

Network::Format::Format(int t_indent, const std::string& t_key) :
    m_indent(t_indent),
    m_value(t_key)
{
}

Network::Format::Format(const std::string& t_delimiter,
                        int t_indent,
                        const std::string& t_key) :
    m_delimiter(t_delimiter),
    m_indent(t_indent),
    m_value(t_key)
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

    if (!format.m_value.empty()) {
        os << format.m_value
           << '=';
    }

    return os;
}
