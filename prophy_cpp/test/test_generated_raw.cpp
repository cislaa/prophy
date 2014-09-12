#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "util.hpp"
#include "generated_raw/ManyArrays.pp.hpp"
#include "generated_raw/ManyArraysMixed.pp.hpp"
#include "generated_raw/ManyArraysMixedHeavily.pp.hpp"
#include "generated_raw/ManyArraysPadding.pp.hpp"
#include "generated_raw/ManyArraysTailFixed.pp.hpp"
#include "generated_raw/ManyDynamic.pp.hpp"

using namespace testing;

TEST(generated_raw, ManyArrays)
{
    data x(
        "\x00\x00\x00\x05"
        "\x01\x02\x03\x04"
        "\x05\xab"
        "\x00\x02\x00\x01"
        "\x00\x02"
        "\x03\xab\xab\xab\xab\xab\xab\xab"
        "\x00\x00\x00\x00\x00\x00\x00\x01"
        "\x00\x00\x00\x00\x00\x00\x00\x02"
        "\x00\x00\x00\x00\x00\x00\x00\x03",

        "\x05\x00\x00\x00"
        "\x01\x02\x03\x04"
        "\x05\xab"
        "\x02\x00\x01\x00"
        "\x02\x00"
        "\x03\xab\xab\xab\xab\xab\xab\xab"
        "\x01\x00\x00\x00\x00\x00\x00\x00"
        "\x02\x00\x00\x00\x00\x00\x00\x00"
        "\x03\x00\x00\x00\x00\x00\x00\x00"
    );

    ManyArrays* next = prophy::swap(reinterpret_cast<ManyArrays*>(x.input.data()));

    EXPECT_EQ(byte_distance(x.input.data(), next), 48);
    EXPECT_THAT(x.input, ContainerEq(x.expected));
}

TEST(generated_raw, ManyArraysMixed)
{
    data x(
        "\x00\x00\x00\x05"
        "\x00\x02"
        "\x01\x02\x03\x04"
        "\x05\x00"
        "\x00\x01\x00\x02",

        "\x05\x00\x00\x00"
        "\x02\x00"
        "\x01\x02\x03\x04"
        "\x05\x00"
        "\x01\x00\x02\x00"
    );

    ManyArraysMixed* next = prophy::swap(reinterpret_cast<ManyArraysMixed*>(x.input.data()));

    EXPECT_EQ(byte_distance(x.input.data(), next), 16);
    EXPECT_THAT(x.input, ContainerEq(x.expected));
}

TEST(generated_raw, ManyArraysMixedHeavily)
{
    data x(
        "\x00\x00\x00\x01"
        "\x00\x00\x00\x03"
        "\x00\x01\x00\x02"
        "\x00\x03\xab\xcd"
        "\x00\x00\x00\x05"
        "\x00\x04\x00\x05"
        "\x00\x06\x00\x07"
        "\x00\x08\xab\xcd"
        "\x00\x09\xab\xcd",

        "\x01\x00\x00\x00"
        "\x03\x00\x00\x00"
        "\x01\x00\x02\x00"
        "\x03\x00\xab\xcd"
        "\x05\x00\x00\x00"
        "\x04\x00\x05\x00"
        "\x06\x00\x07\x00"
        "\x08\x00\xab\xcd"
        "\x09\x00\xab\xcd"
    );

    ManyArraysMixedHeavily* next = prophy::swap(reinterpret_cast<ManyArraysMixedHeavily*>(x.input.data()));

    EXPECT_EQ(byte_distance(x.input.data(), next), 36);
    EXPECT_THAT(x.input, ContainerEq(x.expected));
}

TEST(generated_raw, ManyArraysPadding)
{
    data x(
        "\x01\x00\x00\x00"
        "\x00\x00\x00\x00"
        "\x02\x02\x03\x00"
        "\x00\x00\x00\x02"
        "\x04\x05\x00\x00"
        "\x00\x00\x00\x00"
        "\x00\x00\x00\x00"
        "\x00\x00\x00\x06",

        "\x01\x00\x00\x00"
        "\x00\x00\x00\x00"
        "\x02\x02\x03\x00"
        "\x02\x00\x00\x00"
        "\x04\x05\x00\x00"
        "\x00\x00\x00\x00"
        "\x06\x00\x00\x00"
        "\x00\x00\x00\x00"
    );

    ManyArraysPadding* next = prophy::swap(reinterpret_cast<ManyArraysPadding*>(x.input.data()));

    EXPECT_EQ(byte_distance(x.input.data(), next), 32);
    EXPECT_THAT(x.input, ContainerEq(x.expected));
}

TEST(generated_raw, ManyArraysTailFixed)
{
    data x(
        "\x02\x02\x03\x00"
        "\x00\x00\x00\x00"
        "\x00\x00\x00\x04"
        "\x00\x00\x00\x00"
        "\x00\x00\x00\x00"
        "\x00\x00\x00\x05",

        "\x02\x02\x03\x00"
        "\x00\x00\x00\x00"
        "\x04\x00\x00\x00"
        "\x00\x00\x00\x00"
        "\x05\x00\x00\x00"
        "\x00\x00\x00\x00"
    );

    ManyArraysTailFixed* next = prophy::swap(reinterpret_cast<ManyArraysTailFixed*>(x.input.data()));

    EXPECT_EQ(byte_distance(x.input.data(), next), 24);
    EXPECT_THAT(x.input, ContainerEq(x.expected));
}

TEST(generated_raw, ManyDynamic)
{
    data x(
        "\x00\x00\x00\x01"
        "\x00\x01\xab\xcd"
        "\x00\x00\x00\x02"
        "\x00\x02\x00\x03"
        "\x00\x00\x00\x03"
        "\x00\x04\x00\x05"
        "\x00\x06\xab\xcd",

        "\x01\x00\x00\x00"
        "\x01\x00\xab\xcd"
        "\x02\x00\x00\x00"
        "\x02\x00\x03\x00"
        "\x03\x00\x00\x00"
        "\x04\x00\x05\x00"
        "\x06\x00\xab\xcd"
    );

    ManyDynamic* next = prophy::swap(reinterpret_cast<ManyDynamic*>(x.input.data()));

    EXPECT_EQ(byte_distance(x.input.data(), next), 28);
    EXPECT_THAT(x.input, ContainerEq(x.expected));
}
