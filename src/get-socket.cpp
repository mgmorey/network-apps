#include "network/socket.h"     // Fd, Result, Socket,
                                // SocketpairResult, SocketResult,
                                // operator<<()
#include "network/error.h"      // format_error(), get_last_error(),
                                // set_last_error()
#include "network/format.h"     // Format

#ifdef _WIN32
#include <winsock2.h>   // socket()
#else
#include <sys/socket.h> // socket(), socketpair()
#endif

#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

static const std::string delim {", "};
static const int tab {0};

extern Network::SocketResult Network::get_socket(const Socket& sock,
                                                 bool verbose)
{
    Result result;

    if (verbose) {
        std::cerr << "Calling socket("
                  << Format("domain")
                  << sock.family()
                  << Format(delim, tab, "type")
                  << sock.socktype()
                  << Format(delim, tab, "protocol")
                  << sock.protocol()
                  << ')'
                  << std::endl;
    }

    reset_last_error();
    const auto fd {::socket(sock.family(), sock.socktype(), sock.protocol())};

    if (fd == fd_null) {
        auto error = get_last_error();
        std::ostringstream oss;
        oss << "Call to socket("
            << Format("domain")
            << sock.family()
            << Format(delim, tab, "type")
            << sock.socktype()
            << Format(delim, tab, "protocol")
            << sock.protocol()
            << ") failed with error "
            << error
            << ": "
            << format_error(error);
        return Result(error, oss.str());
    }

    return Fd(fd);
}
