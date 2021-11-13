#ifndef _WIN32

#include "network/socket.h"     // FdPair, Result, Socket,
                                // SocketpairResult operator<<()
#include "network/error.h"      // format_error(), get_last_error(),
                                // set_last_error()
#include "network/format.h"     // Format

#include <sys/socket.h> // socketpair()

#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

static const std::string delim {", "};
static const int tab {0};

extern Network::SocketpairResult Network::get_socketpair(const Socket& sock,
                                                         bool verbose)
{
    Result result;
    fd_type fds[] {fd_null, fd_null};

    if (verbose) {
        std::cerr << "Calling socketpair("
                  << Format("domain")
                  << sock.family()
                  << Format(delim, tab, "type")
                  << sock.socktype()
                  << Format(delim, tab, "protocol")
                  << sock.protocol()
                  << ", ...)"
                  << std::endl;
    }

    reset_last_error();
    auto code {::socketpair(sock.family(), sock.socktype(), sock.protocol(), fds)};

    if (code == socket_error) {
        auto error = get_last_error();
        std::ostringstream oss;
        oss << "Call to socketpair("
            << Format("domain")
            << sock.family()
            << Format(delim, tab, "type")
            << sock.socktype()
            << Format(delim, tab, "protocol")
            << sock.protocol()
            << ", ...) failed with error "
            << error
            << ": "
            << format_error(error);
        return Result(error, oss.str());
    }

    return FdPair(fds[0], fds[1]);
}

#endif
