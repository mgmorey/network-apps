#ifndef NETWORK_TOSTRING_H
#define NETWORK_TOSTRING_H

#include <cstddef>      // std::size_t
#include <iomanip>      // std::hex, std::setfill(), std::setw(),
                        // std::uppercase
#include <sstream>      // std::ostringstream
#include <string>       // std::string

namespace Network
{
    template<typename Container>
    std::string to_string(const Container& value)
    {
        std::string text(value.size(), '\0');

        for (std::string::size_type i = 0; i < value.size(); ++i) {
            text[i] = static_cast<char>(value[i]);
        }

        return text;
    }
}

#endif
