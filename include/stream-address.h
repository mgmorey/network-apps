#ifndef STREAM_ADDRESS_H
#define STREAM_ADDRESS_H

#include "network-address.h"    // Address

#include <ostream>      // std::ostream

namespace Network
{
    extern std::ostream& operator<<(std::ostream& os,
                                    const Address& address);
}

#endif
