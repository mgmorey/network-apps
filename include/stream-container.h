#ifndef STREAM_CONTAINER_H
#define STREAM_CONTAINER_H

#include <iterator>     // std::ostream_iterator
#include <ostream>      // std::ostream

namespace Network
{
    template <class Container>
    std::ostream& operator<<(std::ostream& os, const Container& cont)
    {
        std::copy(cont.begin(), cont.end(),
                  std::ostream_iterator<typename
                  Container::value_type>
                  (os, "\n"));
        return os;
    }
}

#endif
