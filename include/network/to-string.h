#ifndef NETWORK_TO_STRING_H
#define NETWORK_TO_STRING_H

#include <string>       // std::string

namespace Network
{
    template<typename Container>
    auto to_string(const Container& value) -> std::string
    {
        std::string text(value.size(), '\0');

        for (std::string::size_type i = 0; i < value.size(); ++i) {
            text[i] = static_cast<char>(value[i]);
        }

        return text;
    }
}

#endif
