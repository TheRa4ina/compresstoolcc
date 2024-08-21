#pragma once
#include <istream>
#include <unordered_map>
#include <bitset>
#include "node.h"

typedef uint32_t freq_t;
const uint32_t BITSET_SIZE = 8;
typedef std::unordered_map<char, std::bitset<BITSET_SIZE>> CharBitMap;


struct Frequency {
	std::string str;
	freq_t freq;
	Frequency(const std::pair<std::string, freq_t>& pair) {
		str = pair.first;
		freq = pair.second;
	}
	Frequency(const std::string str, const freq_t freq)
		: str(str), freq(freq) {}
	Frequency() {
		str = "";
		freq = 0;
	}
};

namespace huffman{
	CharBitMap buildDictionary(std::istream& is);
}