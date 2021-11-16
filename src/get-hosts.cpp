#include "network/addrinfo.h"       // AddrInfo
#include "network/endpoint.h"       // Endpoint, get_endpoint()
#include "network/hosts.h"          // Hosts, HostsResult, get_hosts()
#include "network/overload.h"       // overload

#include <iterator>     // std::back_inserter()
#include <string>       // std::string
#include <variant>      // std::visit()
#include <vector>       // std::vector

Network::HostsResult Network::get_hosts(const Network::Hostname& host,
                                        const Network::Hints* hints,
                                        bool verbose)
{
    Network::HostsResult hosts_result;
    const auto hostname_result {get_hostname(host)};
    std::visit(Network::Overload {
            [&](const std::string& hostname) {
                Network::Hosts hosts;
                const auto result {
                    Network::AddrInfo::insert(hostname,
                                              nullptr,
                                              hints,
                                              verbose,
                                              std::back_inserter(hosts))
                };

                if (result) {
                    hosts_result = result;
                }
                else {
                    hosts_result = hosts;
                }
            },
            [&](const Network::Result& result) {
                hosts_result = result;
            }
        }, hostname_result);
    return hosts_result;
}
