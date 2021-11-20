#include "network/get-hosts.h"      // Hints, Host, Hostname,
                                    // HostVector, HostVectorResult,
                                    // Result, get_hosts()
#include "network/addrinfo.h"       // AddrInfo
#include "network/get-hostname.h"   // HostnameResult, get_hostname()
#include "network/overload.h"       // overload

#include <iterator>     // std::back_inserter()
#include <optional>     // std::nullopt
#include <string>       // std::string
#include <variant>      // std::visit()

Network::HostVectorResult Network::get_hosts(const Network::Hostname& host,
                                             const Network::Hints* hints,
                                             bool verbose)
{
    HostVectorResult hosts_result;
    const auto hostname_result {get_hostname(host)};
    std::visit(Overload {
            [&](const std::string& hostname) {
                HostVector hosts;
                const auto result {
                    AddrInfo::insert(hostname,
                                     std::nullopt,
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
