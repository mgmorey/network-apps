#include "network/bytestring.h" // operator<<()
#include "network/byte.h"       // Byte
#include "network/print.h"      // print()

#include <ostream>      // std::ostream

std::ostream& Network::operator<<(std::ostream& os,
                                  const ByteString& string)
{
    return print(os, string);
}
