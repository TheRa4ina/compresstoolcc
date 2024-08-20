#include <gtest/gtest.h>
#include <bit>
#include "src/huffman.h"

TEST(Huffman, buildDir_5chars_a0)
{
    Huffman algo;
    //a:15,b:7,c:6,d:6,e:5
    std::stringstream is{ "aaaaaaaaaaaaaaabbbbbbbccccccddddddeeeee" };
    CharBitMap result = algo.buildDictionary(is);
    EXPECT_EQ(std::bit_width(result['a'].to_ulong()), 0);
    EXPECT_EQ(std::bit_width(result['b'].to_ulong()), 3);
    EXPECT_EQ(std::bit_width(result['c'].to_ulong()), 3);
    EXPECT_EQ(std::bit_width(result['d'].to_ulong()), 3);
    EXPECT_EQ(std::bit_width(result['e'].to_ulong()), 3);
    
}
