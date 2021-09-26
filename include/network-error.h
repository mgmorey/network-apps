#ifndef NETWORK_ERROR_H
#define NETWORK_ERROR_H

#include <string>       // std::string

namespace Network
{
    extern std::string format_error(int code);
    extern int get_last_error();
    extern int reset_last_error();
}

#endif
