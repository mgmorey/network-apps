#include "network-connect.h"    // ConnectDetails, ConnectResult,
                                // Hostname, Result, Service, Sockets,
                                // SocketsResult, struct addrinfo,
                                // connect()
#include "network-addrinfo.h"   // operator<<()
#include "network-close.h"      // close()
#include "network-endpoint.h"   // Endpoint, to_string()
#include "network-host.h"       // Host

#ifdef _WIN32
#include <winsock2.h>   // closesocket()
#else
#include <unistd.h>     // close()
#endif

#include <iostream>     // std::cerr, std::endl

Network::ConnectResult Network::connect(const Endpoint& endpoint,
                                        const addrinfo &hints,
                                        bool verbose)
{
    SocketsResult sockets_result(get_sockets(endpoint, hints, verbose));
    Sockets sockets(sockets_result.first);
    Result result(sockets_result.second);

    if (result.nonzero()) {
        ConnectDetails details;
        details.push_back(result);
        return ConnectResult(fd_null, details);
    }

    return connect(sockets, verbose);
}

Network::ConnectResult Network::connect(const Sockets& sockets,
                                        bool verbose)
{
    fd_type fd = fd_null;
    ConnectDetails details;

    for (Sockets::const_iterator it = sockets.begin();
         it != sockets.end();
         ++it) {
        SocketResult socket_result(it->socket(verbose));
        fd = socket_result.first;

        if (fd == fd_null) {
            details.push_back(socket_result.second);
            continue;
        }

        Host host(*it);
        Result connect_result(host.connect(fd, verbose));

        if (connect_result.result() == Host::connect_error) {
            close(fd);
            fd = fd_null;
            details.push_back(connect_result);
        }
        else {
            Result result(0, it->cname());
            details.push_back(result);
            break;
        }
    }

    return ConnectResult(fd, details);
}
