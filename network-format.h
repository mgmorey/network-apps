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
        Format(const std::string& key);
        Format(int indent, const std::string& key = "");
        Format(const std::string& delim, int indent,
               const std::string& key = "");

    private:
        static const std::string del_default;
        static const int ind_default;
        std::string str;
        std::string del;
        int ind;
    };

    extern std::ostream& operator<<(std::ostream& os,
                                    const Format& format);
}

#endif
