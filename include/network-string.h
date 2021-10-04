#ifndef NETWORK_STRING_H
#define NETWORK_STRING_H

#include <cstddef>      // std::size_t
#include <iomanip>      // std::hex, std::setfill(), std::setw(),
                        // std::uppercase
#include <sstream>      // std::ostringstream
#include <string>       // std::string

namespace Network
{
    template<typename Container>
    std::string to_string(const Container& value, bool encode = false)
    {
        if (encode) {
            std::ostringstream oss;
            oss << "0x";

            if (value.empty()) {
                oss << '0';
            }
            else {
                oss << std::hex;

                for (const auto by : value) {
                    oss << std::setfill('0')
                        << std::setw(2)
                        << std::uppercase
                        << static_cast<int>(by);
                }
            }

            return oss.str();
        }
        else {
            std::string text(value.size(), '\0');

            for (std::string::size_type i = 0; i < value.size(); ++i) {
                text[i] = static_cast<char>(value[i]);
            }

            return text;
        }
    }
}

#endif
