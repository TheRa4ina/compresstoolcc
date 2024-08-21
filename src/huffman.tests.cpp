#include <gtest/gtest.h>
#include <bit>
#include "src/huffman.h"

TEST(Huffman, buildDictionary_basicUsage_valid)
{
    //a:15,b:7,c:6,d:6,e:5
    std::stringstream is{ "aaaaaaaaaaaaaaabbbbbbbccccccddddddeeeee" };
    CharBitMap result = huffman::buildDictionary(is);
    EXPECT_EQ(std::bit_width(result['a'].to_ulong()), 0);
    EXPECT_EQ(std::bit_width(result['b'].to_ulong()), 3);
    EXPECT_EQ(std::bit_width(result['c'].to_ulong()), 3);
    EXPECT_EQ(std::bit_width(result['d'].to_ulong()), 3);
    EXPECT_EQ(std::bit_width(result['e'].to_ulong()), 3);
}

TEST(Huffman, buildDictionary_empty_EmptyCharBitMap)
{
    //a:15,b:7,c:6,d:6,e:5
    std::stringstream is{ "" };
    CharBitMap result = huffman::buildDictionary(is);
    ASSERT_TRUE(result.size() == 0);
}