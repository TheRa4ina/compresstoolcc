#include <gtest/gtest.h>
#include <bit>
#include "src/huffman.h"

TEST(Huffman, buildDictionary_basicUsage_valid)
{
    //a:15,b:7,c:6,d:6,e:5
    std::stringstream is{ "aaaaaaaaaaaaaaabbbbbbbccccccddddddeeeee" };
    CharBitMap result = huffman::buildDictionary(is);
    EXPECT_EQ(std::bit_width(result['a']), 0);
    EXPECT_EQ(std::bit_width(result['b']), 3);
    EXPECT_EQ(std::bit_width(result['c']), 3);
    EXPECT_EQ(std::bit_width(result['d']), 3);
    EXPECT_EQ(std::bit_width(result['e']), 3);
}

TEST(Huffman, buildDictionary_empty_EmptyCharBitMap)
{
    //a:15,b:7,c:6,d:6,e:5
    std::stringstream is{ "" };
    CharBitMap result = huffman::buildDictionary(is);
    ASSERT_TRUE(result.size() == 0);
}