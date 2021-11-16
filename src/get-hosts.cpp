#include "network/hosts.h"          // Hints, Host, Hosts, Hostname,
                                    // HostsResult, Result,
                                    // get_hosts()
#include "network/addrinfo.h"       // AddrInfo
#include "network/overload.h"       // overload

#include <iterator>     // std::back_inserter()
#include <string>       // std::string
#include <variant>      // std::visit()

Network::HostsResult Network::get_hosts(const Network::Hostname& host,
                                        const Network::Hints* hints,
                                        bool verbose)
{
    HostsResult hosts_result;
    const auto hostname_result {get_hostname(host)};
    std::visit(Overload {
            [&](const std::string& hostname) {
                Hosts hosts;
                const auto result {
                    AddrInfo::insert(hostname,
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
            [&](const Result& result) {
                hosts_result = result;
            }
        }, hostname_result);
    return hosts_result;
}
