#pragma once
#include <istream>
#include <bit>
#include <functional>
#include <src/node.h>
#include <src/bits.h>
#include <src/util.h>

// FIXME move this structs to some special file or smth

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
	/// <summary>
	/// Builds Huffman dictionary from istream
	/// </summary>
	/// <param name="is">input istream, of which build dictionary</param>
	/// <returns> map of chars to their bitset</returns>
	CharBitMap buildDictionary(CharFreqMap& frequency_map);
}