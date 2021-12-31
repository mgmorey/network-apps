#ifndef NETWORK_GET_PEERNAME_H
#define NETWORK_GET_PEERNAME_H

#include "network/get-name.h"       // BytesResult, Fd

namespace Network
{
    extern auto get_peername(Fd fd, bool verbose = false) -> BytesResult;
}

#endif
