#ifndef NETWORK_GET_HOSTS_H
#define NETWORK_GET_HOSTS_H

#include "network/hints.h"          // Hints
#include "network/hostvector.h"     // HostVector
#include "network/result.h"         // Result
#include "network/types.h"          // Hostname

#include <variant>      // std::variant

namespace Network
{
    using HostVectorResult = std::variant<HostVector, Result>;

    extern HostVectorResult get_hosts(const Hostname& host = nullptr,
                                      const Hints* hints = nullptr,
                                      bool verbose = false);
}

#endif
