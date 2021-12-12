#include "network/bytestring.h" // operator<<()
#include "network/byte.h"       // Byte
#include "network/print.h"      // print()

#include <ostream>      // std::ostream

auto Network::operator<<(std::ostream& os,
                         const ByteString& string) -> std::ostream&
{
    return print(os, string);
}
