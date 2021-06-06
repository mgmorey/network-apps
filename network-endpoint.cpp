#include "network-endpoint.h"

Network::Hostname Network::get_hostname(const Network::Endpoint& endpoint)
{
    return endpoint.first;
}

Network::Service Network::get_service(const Network::Endpoint& endpoint)
{
    return endpoint.second;
}
