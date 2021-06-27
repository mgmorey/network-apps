#include "network-connect.h"    // ConnectDetails, ConnectResult,
                                // Hostname, Result, Service, Sockets,
                                // SocketsResult, struct addrinfo,
                                // close(), connect()
#include "network-addrinfo.h"   // operator<<()
#include "network-endpoint.h"   // Endpoint, to_string()
#include "network-host.h"       // Host

#ifdef _WIN32
#include <winsock2.h>   // closesocket()
#else
#include <unistd.h>     // close()
#endif

#include <iostream>     // std::cerr, std::endl

void Network::close(int fd)
{
    if (fd == -1) {
        return;
    }

#ifdef _WIN32
    ::closesocket(fd);
#else
    ::close(fd);
#endif
}

Network::ConnectResult Network::connect(const Endpoint& endpoint,
                                        const addrinfo &hints,
                                        bool verbose)
{
    SocketsResult socks_result(get_sockets(endpoint, hints, verbose));
    Sockets sockets(socks_result.first);
    Result result(socks_result.second);

    if (result.nonzero()) {
        ConnectDetails details;
        details.push_back(result.string());
        return ConnectResult(Socket::socket_bad, details);
    }

    return connect(sockets, verbose);
}

Network::ConnectResult Network::connect(const Sockets& sockets,
                                        bool verbose)
{
    int fd = Socket::socket_bad;
    ConnectDetails details;

    for (Sockets::const_iterator it = sockets.begin();
         it != sockets.end();
         ++it) {
        if (verbose) {
            std::cerr << "Trying socket:" << std::endl
                      << (*it) << std::endl;
        }

        Result result = it->socket();
        fd = result.result();

        if (fd == Socket::socket_bad) {
            details.push_back(result.string());
            continue;
        }

        result = connect(fd, *it, verbose);

        if (result.result() == Host::connect_error) {
            close(fd);
            fd = Socket::socket_bad;
            details.push_back(result.string());
        }
        else {
            details.push_back(it->cname());
            break;
        }
    }

    return ConnectResult(fd, details);
}

Network::Result Network::connect(int fd, const addrinfo& ai,
                                 bool verbose)
{
    Network::Host host(ai);

    if (verbose) {
        std::cerr << "Trying address: "
                  << to_string(host.address())
                  << std::endl;
    }

    return host.connect(fd);
}
