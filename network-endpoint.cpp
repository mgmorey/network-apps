#include "network-endpoint.h"

#include <string>       // std::string
#include <utility>      // std::pair

Network::Hostname Network::hostname(const Network::Endpoint& endpoint)
{
    return endpoint.first;
}

Network::Service Network::service(const Network::Endpoint& endpoint)
{
    return endpoint.second;
}
