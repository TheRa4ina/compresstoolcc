#pragma once
#include <istream>
#include <unordered_map>
#include <bitset>
#include "node.h"

typedef uint32_t FreqType;
typedef uint8_t Byte;
typedef std::unordered_map<char, Byte> CharBitMap;


struct Frequency {
	std::string str;
	FreqType freq;
	Frequency(const std::pair<std::string, FreqType>& pair) {
		str = pair.first;
		freq = pair.second;
	}
	Frequency(const std::string str, const FreqType freq)
		: str(str), freq(freq) {}
	Frequency() {
		str = "";
		freq = 0;
	}
};

namespace huffman{
	CharBitMap buildDictionary(std::istream& is);
}