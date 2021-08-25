#ifndef STREAM_CONTAINER_H
#define STREAM_CONTAINER_H

#include <algorithm>    // std::copy()
#include <iterator>     // std::ostream_iterator
#include <ostream>      // std::ostream

namespace Network
{
    template <class Container>
    std::ostream& operator<<(std::ostream& os, const Container& cont)
    {
        std::ostream_iterator<typename Container::value_type> output(os, "\n");
        typename Container::const_iterator first(std::begin(cont));
        typename Container::const_iterator last(std::end(cont));
        std::copy(first, last, output);
        return os;
    }
}

#endif
