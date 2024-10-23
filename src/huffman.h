#pragma once
#include <array>
#include <src/bits.h>// for Bits
#include <src/util.h>// for FreqType and charFreqMap

using CharBitMap = std::array<Bits,256> ;
// FIXME width dict seems like a strange name
// considering that everytgin else has a map in a name
// should they even be called maps?


typedef std::array<Width, UINT8_MAX> WidthDict;
// FIXME move this structs to some special file or smth
struct Frequency {
	unsigned char str;
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
	/// Builds character to their codewidth map
	/// </summary>
	/// <param name="frequency_map">frequency of each char</param>
	/// <returns>width of codes map</returns>
	WidthDict buildWidthDictionary(const CharFreqMap& frequency_map);
	/// <summary>
	/// Builds cannonical huffman codes for each char
	/// </summary>
	/// <param name="width_map">widths of each chars</param>
	/// <returns>canonical huffman codes map, even if char not used</returns>
	CharBitMap buildDictionary(const WidthDict& width_map);
	/// <summary>
	/// Builds cannonical huffman codes for each char
	/// </summary>
	/// <param name="frequency_map">frequency of each cahr</param>
	/// <returns>canonical huffman codes map, even if char not used</returns>
	CharBitMap buildDictionary(const CharFreqMap& frequency_map);
	
}