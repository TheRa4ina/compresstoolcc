#include <gtest/gtest.h>
#include <bit>
#include "src/huffman.h"
#include "src/util.h"

TEST(Huffman, getFrequency_basicUsage_valid)
{
    //a:3,b:10,c:6,d:6,e:5
    std::stringstream is{ "aaabbbbbbbbbbccccccddddddeeeee" };
    CharFreqMap freq_map = getFrequency(is);
    EXPECT_EQ(freq_map['a'], 3);
    EXPECT_EQ(freq_map['b'], 10);
    EXPECT_EQ(freq_map['c'], 6);
    EXPECT_EQ(freq_map['d'], 6);
    EXPECT_EQ(freq_map['e'], 5);
}


TEST(Huffman, getFrequency_empty_EmptyCharFreqMap)
{
    std::stringstream is{ "" };
    CharFreqMap freq_map = getFrequency(is);
    ASSERT_TRUE(freq_map.size()==0);
}

TEST(Huffman, buildDictionary_basicUsage_valid)
{
    //nonormal.txt basically, why is it even
    //also width distribution like in wiki canonical huffman example
    std::stringstream is{ "aaaaaaabbbbbbbbbbcccddd" };
    CharFreqMap freq_map = getFrequency(is);
    CharBitMap result = huffman::buildDictionary(freq_map);
    EXPECT_EQ(result['b'], Bits(0b0));
    EXPECT_EQ(result['a'], Bits(0b10));
    EXPECT_EQ(result['c'], Bits(0b110));
    EXPECT_EQ(result['d'], Bits(0b111));
}

