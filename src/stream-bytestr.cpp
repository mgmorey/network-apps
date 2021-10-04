#include "network-bytestr.h"    // ByteString, operator<<()

#include <iomanip>      // std::hex, std::setfill(), std::setw(),
                        // std::uppercase
#include <ostream>      // std::ostream

std::ostream& Network::operator<<(std::ostream& os,
                                  const Network::ByteString& bytes)
{
    os << "0x";

    if (bytes.m_bytes.empty()) {
        os << '0';
    }
    else {
        os << std::hex;

        for (const auto by : bytes.m_bytes) {
            os << std::setfill('0')
               << std::setw(2)
               << std::uppercase
               << static_cast<int>(by);
        }
    }

    return os;
}
