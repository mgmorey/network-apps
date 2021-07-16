#include "network-buffer.h"     // Buffer, std::string

#include <cassert>      // assert()
#include <cstdlib>      // EXIT_SUCCESS
#include <cstring>      // std::memset()

static const std::string::size_type size = 1024;

static void test_buffer()
{
    Network::Buffer buffer(size);
    assert(buffer.size() == size);
    std::string string(buffer);
    assert(string.size() == 0);
    assert(string.find('\0') == std::string::npos);
}

static void test_string()
{
    std::string string(size, '\0');
    assert(string.size() == size);
    std::memset(&string[0], ' ', size / 32);
    assert(string.find('\0') == size / 32);
    std::memset(&string[0], ' ', size);
    assert(string.find('\0') == std::string::npos);
}

int main(void)
{
    test_buffer();
    test_string();
    return EXIT_SUCCESS;
}
