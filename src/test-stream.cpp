#include "stream-container.h"   // print()

#include <cstdlib>      // EXIT_FAILURE, EXIT_SUCCESS
#include <iostream>     // std::cerr, std::cout, std::endl
#include <list>         // std::list
#include <string>       // std::string
#include <vector>       // std::vector

namespace TestStream
{
    static void test_list()
    {
        std::list<std::string> string_list;
        string_list.push_back("one");
        string_list.push_back("two");
        string_list.push_back("three");
        Network::print(std::cout, string_list);
    }

    static void test_vector()
    {
        std::vector<std::string> string_vector;
        string_vector.push_back("one");
        string_vector.push_back("two");
        string_vector.push_back("three");
        Network::print(std::cout, string_vector);
    }
}

int main()
{
    TestStream::test_list();
    TestStream::test_vector();
    return EXIT_SUCCESS;
}
