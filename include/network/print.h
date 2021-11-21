#ifndef NETWORK_PRINT_H
#define NETWORK_PRINT_H

#include <iomanip>      // std::hex, std::setfill(), std::setw(),
                        // std::uppercase
#include <ostream>      // std::ios, std::ostream

namespace Network
{
    template<typename Container>
    std::ostream& print(std::ostream& os, const Container& value)
    {
        std::ios format {nullptr};
        format.copyfmt(os);
        os << "0x";

        if (value.empty()) {
            os << '0';
        }
        else {
            os << std::hex;

            for (const auto by : value) {
                os << std::setfill('0')
                   << std::setw(2)
                   << std::uppercase
                   << static_cast<int>(by);
            }
        }

        os.copyfmt(format);
        return os;
    }
}

#endif
