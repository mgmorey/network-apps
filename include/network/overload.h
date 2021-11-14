#ifndef NETWORK_OVERLOAD_H
#define NETWORK_OVERLOAD_H

namespace Network
{
    template<class... Ts> struct Overload : Ts... { using Ts::operator()...; };
    template<class... Ts> Overload(Ts...) -> Overload<Ts...>; // omit in C++20
}

#endif
