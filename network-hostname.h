#ifndef NETWORK_HOSTNAME_H
#define NETWORK_HOSTNAME_H

#include "network-types.h"      // Hostname

#include <string>       // std::string
#include <utility>      // std::pair

namespace Network
{
    typedef std::pair<Hostname, std::string> HostnameResult;
    extern HostnameResult get_hostname();
}

#endif
