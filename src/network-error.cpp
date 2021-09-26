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

std::string Network::format_error(int code)
{
#ifdef _WIN32
    DWORD dwFlags {
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS
    };
    DWORD dwLanguageId {MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)};
    LPVOID lpMsgBuf {};
    LPTSTR lpBuffer {reinterpret_cast<LPTSTR>(&lpMsgBuf)};
    ::FormatMessage(dwFlags, nullptr, code, dwLanguageId, lpBuffer, 0, nullptr);
    std::string error {static_cast<LPTSTR>(lpMsgBuf)};
    const auto pos {error.find('\n')};
    error = error.substr(0, pos);
    ::LocalFree(lpMsgBuf);
    return error;
#else
    return std::strerror(code);
#endif
}

int Network::get_last_error()
{
#ifdef _WIN32
    return ::WSAGetLastError();
#else
    return errno;
#endif
}

int Network::reset_last_error()
{
    int code {0};
#ifndef _WIN32
    errno = code;
#endif
    return code;
}
