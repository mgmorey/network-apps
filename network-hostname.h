#ifndef NETWORK_HOSTNAME_H
#define NETWORK_HOSTNAME_H

#include "network-result.h"     // Result
#include "network-types.h"      // Hostname

#include <utility>      // std::pair

namespace Network
{
    typedef std::pair<Hostname, Result> HostnameResult;
    extern HostnameResult get_hostname();
}

#endif
