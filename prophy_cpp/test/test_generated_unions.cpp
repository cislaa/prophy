#include <vector>
#include <gtest/gtest.h>
#include "Unions.ppf.hpp"
#include "util.hpp"

using namespace testing;
using namespace prophy::generated;

TEST(generated_unions, Union)
{
    std::vector<char> data(1024);

    Union x{Union::discriminator_a_t, 1};
    size_t size = x.encode(data.data());

    /// encoding
    EXPECT_EQ(12, size);
    EXPECT_EQ(size, x.get_byte_size());
    EXPECT_EQ(bytes(
            "\x01\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00"),
            bytes(data.data(), size));

    x = {Union::discriminator_b_t, 1};
    size = x.encode(data.data());

    EXPECT_EQ(12, size);
    EXPECT_EQ(bytes(
            "\x02\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00"),
            bytes(data.data(), size));

    x = {Union::discriminator_c_t, {{1, 2}}};
    size = x.encode(data.data());

    EXPECT_EQ(12, size);
    EXPECT_EQ(bytes(
            "\x03\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00"),
            bytes(data.data(), size));

    /// decoding
    EXPECT_TRUE(x.decode(bytes(
            "\x01\x00\x00\x00\x04\x00\x00\x00\x00\x00\x00\x00")));
    EXPECT_EQ(Union::discriminator_a, x.discriminator);
    EXPECT_EQ(4, x.a);
    EXPECT_EQ(std::string(
            "a: 4\n"), x.print());

    EXPECT_TRUE(x.decode(bytes(
            "\x02\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00")));
    EXPECT_EQ(Union::discriminator_b, x.discriminator);
    EXPECT_EQ(8, x.b);
    EXPECT_EQ(std::string(
            "b: 8\n"), x.print());

    EXPECT_TRUE(x.decode(bytes(
            "\x03\x00\x00\x00"
            "\x01\x00\x00\x00\x02\x00\x00\x00")));
    EXPECT_EQ(Union::discriminator_c, x.discriminator);
    EXPECT_EQ(1, x.c.x[0]);
    EXPECT_EQ(2, x.c.x[1]);
    EXPECT_EQ(std::string(
            "c {\n"
            "  x: 1\n"
            "  x: 2\n"
            "}\n"), x.print());
}

TEST(generated_unions, UnionDynamic)
{
    std::vector<char> data(1024);
    size_t size{};
    UnionDynamic x{};

    {
        x = {UnionDynamic::discriminator_a_t, 0xA};
        size_t size = x.encode(data.data());

        /// encoding
        EXPECT_EQ(16, size);
        EXPECT_EQ(size, x.get_byte_size());
        EXPECT_EQ(bytes(
            "\x01\x00\x00\x00\x00\x00\x00\x00"
            "\x0A\x00\x00\x00\x00\x00\x00\x00"),
            bytes(data.data(), size));

        /// decoding
        EXPECT_TRUE(x.decode(bytes(
            "\x01\x00\x00\x00\xFF\xFF\xFF\xFF"
            "\x04\xFF\xFF\xFF\xFF\xFF\xFF\xFF")));
        EXPECT_EQ(Union::discriminator_a, x.discriminator);
        EXPECT_EQ(4, x.a);
        EXPECT_EQ(std::string("a: 4\n"), x.print());
    }

    {
        x = {UnionDynamic::discriminator_b_t, {{0xA}}};
        size = x.encode(data.data());
        EXPECT_EQ(16, size);
        EXPECT_EQ(size, x.get_byte_size());

        x = {UnionDynamic::discriminator_b_t, {{0xA, 0xB, 0xC, 0xD}}};
        size_t size = x.encode(data.data());

        /// encoding
        EXPECT_EQ(20, size);
        EXPECT_EQ(size, x.get_byte_size());
        EXPECT_EQ(bytes(
            "\x02\x00\x00\x00\x00\x00\x00\x00"
            "\x04\x00\x00\x00"
            "\x0A\x00\x0B\x00\x0C\x00\x0D\x00"),
            bytes(data.data(), size));

        /// decoding
        EXPECT_TRUE(x.decode(bytes(
            "\x02\x00\x00\x00\xFF\xFF\xFF\xFF"
            "\x03\x00\x00\x00"
            "\x01\x00\x02\x00\x03\x00\xFF\xFF")));
        EXPECT_EQ(Union::discriminator_b, x.discriminator);
        EXPECT_EQ(3, x.b.x.size());
        EXPECT_EQ(1, x.b.x[0]);
        EXPECT_EQ(2, x.b.x[1]);
        EXPECT_EQ(3, x.b.x[2]);
        EXPECT_EQ(std::string(
            "b {\n"
            "  x: 1\n"
            "  x: 2\n"
            "  x: 3\n"
            "}\n"
            ), x.print());
    }

    {
        x = {UnionDynamic::discriminator_c_t, 0x1100AABBCCDDEEFFll};
        size_t size = x.encode(data.data());

        /// encoding
        EXPECT_EQ(16, size);
        EXPECT_EQ(size, x.get_byte_size());
        EXPECT_EQ(bytes(
            "\x03\x00\x00\x00\x00\x00\x00\x00"
            "\xFF\xEE\xDD\xCC\xBB\xAA\x00\x11"),
            bytes(data.data(), size));

        /// decoding
        EXPECT_TRUE(x.decode(bytes(
            "\x03\x00\x00\x00\xFF\xFF\xFF\xFF"
            "\x0A\x00\x00\x00\x00\x00\x00\x00")));
        EXPECT_EQ(Union::discriminator_c, x.discriminator);
        EXPECT_EQ(0xA, x.c);
        EXPECT_EQ(std::string("c: 10\n"), x.print());
    }
}

TEST(generated_unions, BuiltinOptional)
{
    std::vector<char> data(1024);

    BuiltinOptional x{};
    size_t size = x.encode(data.data());

    /// encoding
    EXPECT_EQ(8, size);
    EXPECT_EQ(size, x.get_byte_size());
    EXPECT_EQ(bytes(
            "\x00\x00\x00\x00\x00\x00\x00\x00"),
            bytes(data.data(), size));

    x = {2};
    size = x.encode(data.data());

    EXPECT_EQ(8, size);
    EXPECT_EQ(bytes(
            "\x01\x00\x00\x00\x02\x00\x00\x00"),
            bytes(data.data(), size));

    /// decoding
    EXPECT_TRUE(x.decode(bytes(
            "\x00\x00\x00\x00\x00\x00\x00\x00")));
    EXPECT_FALSE(x.x);
    EXPECT_EQ(std::string(
            ""), x.print());

    EXPECT_TRUE(x.decode(bytes(
            "\x01\x00\x00\x00\x05\x00\x00\x00")));
    EXPECT_TRUE(x.x);
    EXPECT_EQ(5, *x.x);
    EXPECT_EQ(std::string(
            "x: 5\n"), x.print());
}

TEST(generated_unions, FixcompOptional)
{
    std::vector<char> data(1024);

    FixcompOptional x{};
    size_t size = x.encode(data.data());

    /// encoding
    EXPECT_EQ(8, size);
    EXPECT_EQ(size, x.get_byte_size());
    EXPECT_EQ(bytes(
            "\x00\x00\x00\x00\x00\x00\x00\x00"),
            bytes(data.data(), size));

    x = {{{3, 4}}};
    size = x.encode(data.data());

    EXPECT_EQ(8, size);
    EXPECT_EQ(bytes(
            "\x01\x00\x00\x00\x03\x00\x04\x00"),
            bytes(data.data(), size));

    /// decoding
    EXPECT_TRUE(x.decode(bytes(
            "\x00\x00\x00\x00\x00\x00\x00\x00")));
    EXPECT_FALSE(x.x);
    EXPECT_EQ(std::string(
            ""), x.print());

    EXPECT_TRUE(x.decode(bytes(
            "\x01\x00\x00\x00\x07\x00\x08\x00")));
    EXPECT_TRUE(x.x);
    EXPECT_EQ(7, x.x->x);
    EXPECT_EQ(8, x.x->y);
    EXPECT_EQ(std::string(
            "x {\n"
            "  x: 7\n"
            "  y: 8\n"
            "}\n"), x.print());
}
