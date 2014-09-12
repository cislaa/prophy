#include <gtest/gtest.h>

#include "generated_raw/Arrays.ppr.hpp"

using namespace testing;
using namespace raw;

template <class T, size_t N>
static void test_swap(const char (&input) [N], const char (&expected) [N], size_t expected_size = N - 1)
{
    std::string input_(input, N - 1);
    T* next = prophy::swap(reinterpret_cast<T*>(input_.begin().base()));

    EXPECT_EQ(expected_size, reinterpret_cast<char*>(next) - input_.data());
    EXPECT_EQ(std::string(expected, N - 1), std::string(input_, 0, N - 1));
}

TEST(generated_raw_arrays, Builtin)
{
    test_swap<Builtin>(
        "\x01\x00\x00\x02",
        "\x01\x00\x02\x00"
    );
}

TEST(generated_raw_arrays, BuiltinFixed)
{
    test_swap<BuiltinFixed>(
        "\x00\x02"
        "\x00\x02"
        "\x00\x02",

        "\x02\x00"
        "\x02\x00"
        "\x02\x00"
    );
}

TEST(generated_raw_arrays, BuiltinDynamic)
{
    test_swap<BuiltinDynamic>(
        "\x00\x00\x00\x03"
        "\x00\x05\x00\x06"
        "\x00\x07\xab\xcd",

        "\x03\x00\x00\x00"
        "\x05\x00\x06\x00"
        "\x07\x00\xab\xcd"
    );
}

TEST(generated_raw_arrays, BuiltinLimited)
{
    test_swap<BuiltinLimited>(
        "\x00\x00\x00\x02"
        "\x00\x05\x00\x06"
        "\xab\xcd\xef\xba",

        "\x02\x00\x00\x00"
        "\x05\x00\x06\x00"
        "\xab\xcd\xef\xba"
    );
}

TEST(generated_raw_arrays, BuiltinGreedy)
{
    test_swap<BuiltinGreedy>(
        "\x00\x08\xab\xcd"
        "\x00\x00\x00\x01"
        "\x00\x00\x00\x02",

        "\x08\x00\xab\xcd"
        "\x00\x00\x00\x01"
        "\x00\x00\x00\x02",

        4
    );
}

TEST(generated_raw_arrays, Fixcomp)
{
    test_swap<Fixcomp>(
        "\x01\x00\x00\x02"
        "\x01\x00\x00\x02",

        "\x01\x00\x02\x00"
        "\x01\x00\x02\x00"
    );
}

TEST(generated_raw_arrays, FixcompFixed)
{
    test_swap<FixcompFixed>(
        "\x01\x00\x00\x02"
        "\x01\x00\x00\x02"
        "\x01\x00\x00\x02",

        "\x01\x00\x02\x00"
        "\x01\x00\x02\x00"
        "\x01\x00\x02\x00"
    );
}

TEST(generated_raw_arrays, FixcompDynamic)
{
    test_swap<FixcompDynamic>(
        "\x00\x00\x00\x03"
        "\x01\x00\x00\x01"
        "\x02\x00\x00\x02"
        "\x03\x00\x00\x03",

        "\x03\x00\x00\x00"
        "\x01\x00\x01\x00"
        "\x02\x00\x02\x00"
        "\x03\x00\x03\x00"
    );
}

TEST(generated_raw_arrays, FixcompLimited)
{
    test_swap<FixcompLimited>(
        "\x00\x02"
        "\x01\x00\x00\x01"
        "\x02\x00\x00\x02"
        "\xab\xcd\xef\xba",

        "\x02\x00"
        "\x01\x00\x01\x00"
        "\x02\x00\x02\x00"
        "\xab\xcd\xef\xba"
    );
}

TEST(generated_raw_arrays, FixcompGreedy)
{
    test_swap<FixcompGreedy>(
        "\x00\x01"
        "\x01\x00\x00\x01"
        "\x02\x00\x00\x02"
        "\x01\x00\x00\x01"
        "\x02\x00\x00\x02",

        "\x01\x00"
        "\x01\x00\x00\x01"
        "\x02\x00\x00\x02"
        "\x01\x00\x00\x01"
        "\x02\x00\x00\x02",

        2
    );
}

TEST(generated_raw_arrays, Dyncomp)
{
    test_swap<Dyncomp>(
        "\x00\x00\x00\x03"
        "\x00\x01\x00\x02"
        "\x00\x03\xab\xcd",

        "\x03\x00\x00\x00"
        "\x01\x00\x02\x00"
        "\x03\x00\xab\xcd"
    );
}

TEST(generated_raw_arrays, DyncompDynamic)
{
    test_swap<DyncompDynamic>(
        "\x00\x00\x00\x02"
        "\x00\x00\x00\x01"
        "\x00\x01\xef\xab"
        "\x00\x00\x00\x03"
        "\x00\x01\x00\x02"
        "\x00\x03\xab\xcd",

        "\x02\x00\x00\x00"
        "\x01\x00\x00\x00"
        "\x01\x00\xef\xab"
        "\x03\x00\x00\x00"
        "\x01\x00\x02\x00"
        "\x03\x00\xab\xcd"
    );
}

TEST(generated_raw_arrays, DyncompGreedy)
{
    test_swap<DyncompGreedy>(
        "\x00\x01\xab\xcd"
        "\x00\x00\x00\x01"
        "\x00\x01\xef\xab"
        "\x00\x00\x00\x03"
        "\x00\x01\x00\x02"
        "\x00\x03\xab\xcd",

        "\x01\x00\xab\xcd"
        "\x00\x00\x00\x01"
        "\x00\x01\xef\xab"
        "\x00\x00\x00\x03"
        "\x00\x01\x00\x02"
        "\x00\x03\xab\xcd",

        4
    );
}
