#include "network/get-endpoint.h"       // Endpoint, EndpointResult,
                                        // Result, get_endpoint()
#include "network/buffer.h"             // Buffer
#include "network/get-length.h"         // SockAddr, get_length()
#include "network/get-pointer.h"        // SockAddr, get_pointer()
#include "network/is-valid.h"           // SockAddr, is_valid()

#ifdef _WIN32
#include <ws2tcpip.h>   // NI_MAXHOST, NI_MAXSERV, gai_strerror(),
                        // getnameinfo()
#else
#include <netdb.h>      // NI_MAXHOST, NI_MAXSERV, gai_strerror(),
                        // getnameinfo()
#endif

#include <algorithm>    // std::max()
#include <cassert>      // assert()
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream
#include <string>       // std::string

auto Network::get_endpoint(const SockAddr& addr, int flags, bool verbose) ->
    Network::EndpointResult
{
    Result result;
    Buffer host {NI_MAXHOST};
    Buffer serv {NI_MAXSERV};
    assert(is_valid(addr, verbose));  // NOLINT
    const auto addr_len {get_length(addr)};
    const auto *const addr_ptr {get_pointer(addr)};

    if (verbose) {
        std::cerr << "Calling getnameinfo("
                  << addr
                  << ", "
                  << addr_len
                  << ", ..., "
                  << flags
                  << ')'
                  << std::endl;
    }

    const auto error {::getnameinfo(addr_ptr, addr_len,
                                    host.data(), host.size(),
                                    serv.data(), serv.size(),
                                    flags)};

    if (error != 0) {
        std::ostringstream oss;
        oss << "Call to getnameinfo("
            << addr
            << ", "
            << addr_len
            << ", ..., "
            << flags
            << ") returned "
            << error
            << " ("
            << ::gai_strerror(error)
            << ')';
        return Result(error, oss.str());
    }

    return Endpoint(host, serv);
}
