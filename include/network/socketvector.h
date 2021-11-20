#ifndef NETWORK_SOCKETVECTOR_H
#define NETWORK_SOCKETVECTOR_H

#include "network/socket.h"         // Socket

#include <vector>       // std::vector

namespace Network
{
    using SocketVector = std::vector<Socket>;
}

#endif
