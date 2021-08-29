#include "stream-container.h"   // print()

#include <iostream>     // std::cerr, std::cout, std::endl
#include <list>         // std::list
#include <string>       // std::string
#include <vector>       // std::vector

namespace TestStream
{
    static void test_list()
    {
        std::list<std::string> strings {"one", "two", "three"};
        Network::print(strings, std::cout);
    }

    static void test_vector()
    {
        std::vector<std::string> strings {"one", "two", "three"};
        Network::print(strings, std::cout);
    }
}

int main()
{
    TestStream::test_list();
    TestStream::test_vector();
}
