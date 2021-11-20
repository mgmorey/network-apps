#ifndef _WIN32

#include "network/socket.h"     // FdArray, FdArrayResult, Result,
                                // Socket, fd_null, fd_type,
                                // operator<<()
#include "network/error.h"      // format_error(), get_last_error(),
                                // set_last_error()
#include "network/format.h"     // Format

#include <sys/socket.h> // socketpair()

#include <array>        // std::array
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

static const std::string delim {", "};
static const int tab {0};

extern Network::FdArrayResult Network::get_socketpair(const Socket& sock,
                                                      bool verbose)
{
    Result result;
    std::array<fd_type, 2> fds {fd_null, fd_null};

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
    auto code {::socketpair(sock.family(),
                            sock.socktype(),
                            sock.protocol(),
                            fds.data())};

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

    return FdArray {fds[0], fds[1]};
}

#endif
