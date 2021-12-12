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
#ifdef _WIN32
    LPVOID buffer {nullptr};
    const DWORD flags {
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS
    };
    const DWORD lang {MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)};
    const LPTSTR pbuffer {reinterpret_cast<LPTSTR>(&buffer)};

    if(::FormatMessage(flags, nullptr, error, lang, pbuffer, 0, nullptr)) {
        std::string message {static_cast<LPTSTR>(buffer)};
        const auto pos {message.find('\n')};
        message = message.substr(0, pos);
        ::LocalFree(buffer);
        return message;
    }
    else {
        return "";
    }
#else
    return std::strerror(error);
#endif
}

auto Network::get_last_error() -> Network::error_type
{
#ifdef _WIN32
    return ::WSAGetLastError();
#else
    return errno;
#endif
}

auto Network::reset_last_error() -> Network::error_type
{
    error_type error {0};
#ifndef _WIN32
    errno = error;
#endif
    return error;
}
