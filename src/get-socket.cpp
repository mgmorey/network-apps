#include "network/get-socket.h"     // Fd, FdResult, Result, Socket,
                                    // fd_null, get_socket(), operator<<()
#include "network/error.h"          // format_error(), get_last_error(),
                                    // set_last_error()
#include "network/format.h"         // Format

#ifdef _WIN32
#include <winsock2.h>       // socket()
#else
#include <sys/socket.h>     // socket()
#endif

#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

static constexpr auto delim {", "};
static constexpr auto tab {0};

auto Network::get_socket(const Socket& sock,
                         bool verbose) -> Network::FdResult
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
