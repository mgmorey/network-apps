#ifndef NETWORK_GTEST_HPP
#define NETWORK_GTEST_HPP

#if defined(__has_include)
# if __has_include(<gtest/gtest.h>)
#  include <gtest/gtest.h>
#  define NETWORK_HAS_GTEST 1
# endif
#endif

#ifndef NETWORK_HAS_GTEST

#include <cstdlib>
#include <iostream>
#include <utility>

#define TEST(suite_name, test_name)                                        \
    static void suite_name##_##test_name();                                \
    static const int suite_name##_##test_name##_registered                \
    {[]() { suite_name##_##test_name(); return 0; }()};                   \
    static void suite_name##_##test_name()

#define EXPECT_TRUE(value)                                                 \
    do {                                                                   \
        const auto network_test_value = static_cast<bool>(value);          \
        if (!network_test_value) {                                         \
            std::cerr << "EXPECT_TRUE failed\n";                           \
            std::exit(EXIT_FAILURE);                                       \
        }                                                                  \
    } while (false)

#define EXPECT_FALSE(value)                                                \
    do {                                                                   \
        const auto network_test_value = static_cast<bool>(value);          \
        if (network_test_value) {                                          \
            std::cerr << "EXPECT_FALSE failed\n";                          \
            std::exit(EXIT_FAILURE);                                       \
        }                                                                  \
    } while (false)

#define EXPECT_EQ(actual, expected)                                        \
    do {                                                                   \
        const auto network_test_actual = (actual);                         \
        const auto network_test_expected = (expected);                     \
        if (!(network_test_actual == network_test_expected)) {             \
            std::cerr << "EXPECT_EQ failed\n";                             \
            std::exit(EXIT_FAILURE);                                       \
        }                                                                  \
    } while (false)

#define EXPECT_LE(actual, upper)                                           \
    do {                                                                   \
        const auto network_test_actual = (actual);                         \
        const auto network_test_upper = (upper);                           \
        if (!(network_test_actual <= network_test_upper)) {                \
            std::cerr << "EXPECT_LE failed\n";                             \
            std::exit(EXIT_FAILURE);                                       \
        }                                                                  \
    } while (false)

#define EXPECT_GT(actual, lower)                                           \
    do {                                                                   \
        const auto network_test_actual = (actual);                         \
        const auto network_test_lower = (lower);                           \
        if (!(network_test_actual > network_test_lower)) {                 \
            std::cerr << "EXPECT_GT failed\n";                             \
            std::exit(EXIT_FAILURE);                                       \
        }                                                                  \
    } while (false)

#define EXPECT_NE(actual, expected)                                        \
    do {                                                                   \
        const auto network_test_actual = (actual);                         \
        const auto network_test_expected = (expected);                     \
        if (network_test_actual == network_test_expected) {                \
            std::cerr << "EXPECT_NE failed\n";                             \
            std::exit(EXIT_FAILURE);                                       \
        }                                                                  \
    } while (false)

#define ASSERT_FALSE(value) EXPECT_FALSE(value)
#define ASSERT_TRUE(value) EXPECT_TRUE(value)
#define ASSERT_EQ(actual, expected) EXPECT_EQ(actual, expected)
#define ASSERT_NE(actual, expected)                                        \
    do {                                                                   \
        const auto network_test_actual = (actual);                         \
        const auto network_test_expected = (expected);                     \
        if (network_test_actual == network_test_expected) {                \
            std::cerr << "ASSERT_NE failed\n";                             \
            std::exit(EXIT_FAILURE);                                       \
        }                                                                  \
    } while (false)

auto main() -> int
{
    return EXIT_SUCCESS;
}

#endif

#endif
