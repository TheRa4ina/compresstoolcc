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
};


class Huffman {
public:
	CharBitMap buildDictionary(std::istream& is);
private:
	std::unordered_map<std::string, freq_t> getFrequency(std::istream& is);
	Node<Frequency> buildTree(std::unordered_map<std::string,freq_t> char_frequency);
	CharBitMap helperBuildDictionary(const std::shared_ptr<const Node<Frequency>> cur_node,std::bitset<BITSET_SIZE> cur_bits) const;

	static constexpr std::size_t CHUNK_SIZE = 1024;
};