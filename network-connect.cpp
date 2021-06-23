#include "network-connect.h"    // ConnectDetails, ConnectResult,
                                // Hostname, Result, Service, struct
                                // addrinfo, close_socket(),
                                // connect_socket()
#include "network-address.h"    // Address
#include "network-addrinfo.h"   // operator<<()
#include "network-endpoint.h"   // operator<<()
#include "network-sockets.h"    // Sockets

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

Network::ConnectResult Network::connect(const Hostname& host,
                                        const Service& service,
                                        const struct addrinfo &hints,
                                        bool verbose)
{
    SocketsResult socks_result(get_sockets(host, service, hints, verbose));
    Sockets sockets(socks_result.first);
    Result result(socks_result.second);
    int fd = Socket::socket_bad;
    ConnectDetails details;

    if (result.nonzero()) {
        details.push_back(result.string());
        return ConnectResult(fd, details);
    }

    for (Sockets::const_iterator it = sockets.begin();
         it != sockets.end();
         ++it) {
        if (verbose) {
            std::cerr << "Trying socket:" << std::endl
                      << (*it) << std::endl;
        }

        result = it->socket();
        fd = result.result();

        if (fd == Socket::socket_bad) {
            details.push_back(result.string());
            continue;
        }

        result = connect(fd, *it, verbose);

        if (result.result() == Address::connect_error) {
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

Network::Result Network::connect(int fd, const struct addrinfo& ai,
                                 bool verbose)
{
    Network::Address address(ai);

    if (verbose) {
        std::cerr << "Trying address: "
                  << to_string(address)
                  << std::endl;
    }

    return address.connect(fd);
}
