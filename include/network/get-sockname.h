#ifndef NETWORK_GET_SOCKNAME_H
#define NETWORK_GET_SOCKNAME_H

#include "network/get-name.h"       // BytesResult, Fd

namespace Network
{
    extern auto get_sockname(Fd fd, bool verbose = false) -> BytesResult;
}

#endif
