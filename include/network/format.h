#ifndef NETWORK_FORMAT_H
#define NETWORK_FORMAT_H

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{

    class Format
    {
        friend std::ostream& operator<<(std::ostream& os,
                                        const Format& format);

    public:
        explicit Format(const std::string& t_key);
        explicit Format(int t_indent, const std::string& t_key = "");
        Format(const std::string& t_delimiter, int indent,
               const std::string& key = "");

    private:
        static constexpr auto m_delimiter_default = ",";
        static constexpr auto m_indent_default = -1;
        std::string m_value;
        std::string m_delimiter;
        int m_indent;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Format& format);
}

#endif
