#include "network/print.h"      // print()
#include "network/byte.h"       // Byte, operator<<()

#include <ostream>      // std::ostream

std::ostream& Network::operator<<(std::ostream& os,
                                  const ByteString& string)
{
    return print(os, string);
}
