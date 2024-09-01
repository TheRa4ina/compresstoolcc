#include <gtest/gtest.h>
#include <bit>
#include "src/huffman.h"
#include "src/util.h"

TEST(Huffman, buildDictionary_basicUsage_valid)
{
    //a:15,b:7,c:6,d:6,e:5
    std::stringstream is{ "aaaaaaaaaaaaaaabbbbbbbccccccddddddeeeee" };
    CharFreqMap freq_map = getFrequency(is);
    CharBitMap result = huffman::buildDictionary(freq_map);
    EXPECT_EQ(result['a'].getWidth(), 1);
    EXPECT_EQ(result['b'].getWidth(), 3);
    EXPECT_EQ(result['c'].getWidth(), 3);
    EXPECT_EQ(result['d'].getWidth(), 3);
    EXPECT_EQ(result['e'].getWidth(), 3);
}

TEST(Huffman, buildDictionary_empty_EmptyCharBitMap)
{
    std::stringstream is{ "" };
    CharFreqMap freq_map = getFrequency(is);
    CharBitMap result = huffman::buildDictionary(freq_map);
    ASSERT_TRUE(result.size() == 0);
}