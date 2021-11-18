#ifndef NETWORK_ERROR_H
#define NETWORK_ERROR_H

#ifdef _WIN32
#include <winsock2.h>       // SOCKET_ERROR
#include <windows.h>        // DWORD
#endif

#include <string>       // std::string

namespace Network
{
#ifdef _WIN32
    using error_type = DWORD;
#else
    using error_type = int;
#endif

#ifndef SOCKET_ERROR
    constexpr error_type SOCKET_ERROR {-1};
#endif

    enum { socket_error = SOCKET_ERROR };
    extern std::string format_error(error_type error);
    extern error_type get_last_error();
    extern error_type reset_last_error();
}

#endif
