#include "network/error.h"      // get_last_error(),
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

auto Network::format_error(error_type error) -> std::string
{
    std::string message;
#ifdef _WIN32
    LPVOID buffer {nullptr};
    const DWORD flags {
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS
    };
    const DWORD lang {MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)};
    const LPVOID pbuffer = &buffer;
    const LPTSTR pstring {static_cast<LPTSTR>(pbuffer)};

    if(::FormatMessage(flags, nullptr, error, lang, pstring, 0, nullptr)) {
        message = static_cast<LPTSTR>(buffer);
        const auto pos {message.find('\n')};
        message = message.substr(0, pos);
        ::LocalFree(buffer);
    }
#else
    message = std::strerror(error);
#endif
    return message;
}

auto Network::get_last_error() -> Network::error_type
{
    error_type error {0};
#ifdef _WIN32
    error = ::WSAGetLastError();
#else
    error = errno;
#endif
    return error;
}

auto Network::reset_last_error() -> Network::error_type
{
    error_type error {0};
#ifndef _WIN32
    errno = error;
#endif
    return error;
}
