#ifndef NETWORK_ERROR_H
#define NETWORK_ERROR_H

#ifdef _WIN32
#include <winsock2.h>   // Must include winsock2.h before windows.h
#include <windows.h>    // DWORD
#endif

#include <string>       // std::string

namespace Network
{
#ifdef _WIN32
    typedef DWORD error_type;
#else
    typedef int error_type;
#endif

    extern std::string format_error(error_type code);
    extern error_type get_last_error();
    extern error_type reset_last_error();
}

#endif
