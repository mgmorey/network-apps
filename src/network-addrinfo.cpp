#include "network-addrinfo.h"   // AddrInfoResult, Hostname, Service,
                                // Result, struct addrinfo,
                                // copy_addrinfo(), get_addrinfo(),
                                // std::cerr, std::endl, std::ostream,
                                // std::size_t, std::string
#include "network-address.h"    // Address, operator<<()
#include "network-family.h"     // Family, operator<<()
#include "network-flags.h"      // Flags, operator<<()
#include "network-format.h"     // Format, operator<<()
#include "network-nullable.h"   // Nullable, operator<<()
#include "network-protocol.h"   // Protocol, operator<<()
#include "network-socktype.h"   // SockType, operator<<()

#ifdef _WIN32
#include <ws2tcpip.h>   // struct addrinfo, getaddrinfo()
#else
#include <netdb.h>      // struct addrinfo, getaddrinfo()
#endif

#include <cassert>      // assert()
#include <ostream>      // std::ostream
#include <sstream>      // std::ostringstream
#include <string>       // std::string

Network::AddrInfoResult Network::get_addrinfo(const Hostname& node,
                                              const Service& service,
                                              const addrinfo* hints,
                                              bool verbose)
{
    if (verbose) {
        std::cerr << "Invoking getaddrinfo("
                  << Nullable(node)
                  << ", "
                  << Nullable(service)
                  << ", "
                  << hints
                  << ", ...)"
                  << std::endl
                  << "Using addrinfo hints "
                  << hints
                  << ':'
                  << std::endl
                  << *hints
                  << std::endl;
    }

    std::string error;
    addrinfo* list = nullptr;
    assert(!node.empty() || !service.empty());
    const int code = ::getaddrinfo(node.empty() ? nullptr : node.c_str(),
                                   service.empty() ? nullptr : service.c_str(),
                                   hints,
                                   &list);

    if (code != 0) {
        std::ostringstream oss;
        oss << "getaddrinfo("
            << Nullable(node)
            << ", "
            << Nullable(service)
            << ", "
            << hints
            << ") returned "
            << code
            << ": "
            << ::gai_strerror(code);
        error = oss.str();
    }
    else {
        assert(list != nullptr);
    }

    Result result(code, error);
    assert(result.result() ?
           result.string() != "" :
           result.string() == "");
    return AddrInfoResult(list, result);
}
