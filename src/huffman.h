#pragma once
#include <istream>
#include <unordered_map>
#include <bitset>
#include "node.h"

typedef uint32_t FreqType;
typedef uint32_t Bits;
typedef std::unordered_map<char, Bits> CharBitMap;

//Questionable postion for CharFreqMap?
typedef std::unordered_map<char, FreqType> CharFreqMap;

struct Frequency {
	char str;
	FreqType freq;
	Frequency(const std::pair<char, FreqType>& pair) {
		str = pair.first;
		freq = pair.second;
	}
	Frequency(const char str, const FreqType freq)
		: str(str), freq(freq) {}
	Frequency() {
		str = '\0';
		freq = 0;
	}
};

namespace huffman{
	CharBitMap buildDictionary(CharFreqMap& frequency_map);
}