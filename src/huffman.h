#pragma once
#include <istream>
#include <unordered_map>
#include <bit>
#include <functional>
#include "node.h"

// TODO move this structs to some special file or smth

struct Bits {
	uint32_t bits;
	uint8_t width;

	Bits()
		: bits(0), width(1) {}
	Bits(const uint32_t value)
		: bits(value), width(std::bit_width(bits)) {}
	Bits(const uint32_t value, uint8_t width)
		: bits(value), width(width) {}
	friend Bits operator<< (Bits i,uint32_t shift) {
		i.bits = i.bits << shift;
		i.width += shift;
		return i;
	}
	Bits& operator++(){
		++bits;
		if (std::has_single_bit(bits)&& bits!=1) {
			++width;
		}
		return *this;
	}
	Bits operator++(int) {
		Bits old = *this;
		operator++();
		return old;
	}

	Bits& operator<<= (uint32_t shift) {
		*this = *this << shift;
		return *this;
	}
	friend Bits operator>> (Bits i, uint32_t shift) {
		i.bits = i.bits >> shift;
		i.width -= shift;
		return i;
	}
	Bits& operator>>= (uint32_t shift) {
		*this= *this >> shift;
		return *this;
	}
	friend Bits operator| (Bits& i, uint32_t mask) {
		i.bits = i.bits | mask;
		return i;
	}
	Bits& operator|= (uint32_t mask) {
		*this= *this|mask;
		return *this;
	}

	friend Bits operator& (Bits& i, uint32_t mask) {
		i.bits = i.bits & mask;
		return i;
	}
	Bits& operator&= (uint32_t mask) {
		*this = *this & mask;
		return *this;
	}
};


namespace std {
	template <>
	struct hash<Bits> {
		size_t operator()(const Bits& b) const noexcept {
			// Combine the hashes of bits and width
			size_t h1 = std::hash<uint32_t>{}(b.bits);
			size_t h2 = std::hash<uint8_t>{}(b.width);
			return h1 ^ (h2 << 1); // or use any other hash combining method
		}
	};
}

typedef std::unordered_map<char, Bits> CharBitMap;

typedef uint32_t FreqType;
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
	/// <summary>
	/// Builds Huffman dictionary from istream
	/// </summary>
	/// <param name="is">input istream, of which build dictionary</param>
	/// <returns> map of chars to their bitset</returns>
	CharBitMap buildDictionary(CharFreqMap& frequency_map);
}