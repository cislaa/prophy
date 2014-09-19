#include <gtest/gtest.h>
#include "util.hpp"
#include "generated_raw/Unions.ppr.hpp"

using namespace testing;
using namespace raw;

TEST(generated_raw_unions, Optional_set)
{
    test_swap<Optional>(
        "\x00\x00\x00\x01"
        "\x00\x00\x00\x01"
        "\x00\x00\x00\x01"
        "\x02\x00\x00\x03"
        "\x04\x00\x00\x05",

        "\x01\x00\x00\x00"
        "\x01\x00\x00\x00"
        "\x01\x00\x00\x00"
        "\x02\x00\x03\x00"
        "\x04\x00\x05\x00"
    );;
}

TEST(generated_raw_unions, Optional_not_set)
{
    test_swap<Optional>(
        "\x00\x00\x00\x00"
        "\xab\xcd\xef\xab"
        "\x00\x00\x00\x00"
        "\xab\xcd\xef\xab"
        "\xab\xcd\xef\xab",

        "\x00\x00\x00\x00"
        "\xab\xcd\xef\xab"
        "\x00\x00\x00\x00"
        "\xab\xcd\xef\xab"
        "\xab\xcd\xef\xab"
    );
}

TEST(generated_raw_unions, Union_a)
{
    test_swap<Union>(
        "\x00\x00\x00\x01"
        "\xab\xcd\xef\xab"
        "\x01\x00\x00\x00"
        "\x00\x00\x00\x00",

        "\x01\x00\x00\x00"
        "\xab\xcd\xef\xab"
        "\x01\x00\x00\x00"
        "\x00\x00\x00\x00"
    );
}

TEST(generated_raw_unions, Union_b)
{
    test_swap<Union>(
        "\x00\x00\x00\x02"
        "\xab\xcd\xef\xab"
        "\x00\x00\x00\x00"
        "\x00\x00\x00\x01",

        "\x02\x00\x00\x00"
        "\xab\xcd\xef\xab"
        "\x01\x00\x00\x00"
        "\x00\x00\x00\x00"
    );
}

TEST(generated_raw_unions, Union_c)
{
    test_swap<Union>(
        "\x00\x00\x00\x03"
        "\xab\xcd\xef\xab"
        "\x01\x00\x00\x02"
        "\x03\x00\x00\x04",

        "\x03\x00\x00\x00"
        "\xab\xcd\xef\xab"
        "\x01\x00\x02\x00"
        "\x03\x00\x04\x00"
    );
}