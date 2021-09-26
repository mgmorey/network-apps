#include "network-error.h"      // get_last_error(),
                                // reset_last_error(), std::string

#ifdef _WIN32
#include <winsock2.h>   // WSAGetLastError()
#include <windows.h>    // FORMAT_MESSAGE*, LANG_NEUTRAL,
                        // MAKELANGID(), SUBLANG_DEFAULT,
                        // FormatMessage(), LocalFree()
#else
#include <cerrno>       // errno
#include <cstring>      // std::strerror()
#endif

std::string Network::format_error(error_type code)
{
#ifdef _WIN32
    LPVOID buffer {nullptr};
    const DWORD flags {
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS
    };
    const DWORD lang {MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)};
    const LPTSTR pbuffer {reinterpret_cast<LPTSTR>(&buffer)};

    if(::FormatMessage(flags, nullptr, code, lang, pbuffer, 0, nullptr)) {
        std::string error {static_cast<LPTSTR>(buffer)};
        const auto pos {error.find('\n')};
        error = error.substr(0, pos);
        ::LocalFree(buffer);
        return error;
    }
    else {
        return "";
    }
#else
    return std::strerror(code);
#endif
}

Network::error_type Network::get_last_error()
{
#ifdef _WIN32
    return ::WSAGetLastError();
#else
    return errno;
#endif
}

Network::error_type Network::reset_last_error()
{
    error_type code {0};
#ifndef _WIN32
    errno = code;
#endif
    return code;
}
