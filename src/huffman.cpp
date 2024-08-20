#include "src/huffman.h"
#include <algorithm>
#include <queue>
#include <vector>
#include <cassert>

namespace {
	std::unordered_map<std::string, freq_t> getFrequency(std::istream& is);
	Node<Frequency> buildTree(std::unordered_map<std::string, freq_t> char_frequency);
	CharBitMap helperBuildDictionary(const std::shared_ptr<const Node<Frequency>> cur_node, std::bitset<BITSET_SIZE> cur_bits);
}

/// <summary>
/// Builds Huffman dictionary from istream
/// </summary>
/// <param name="is">input istream, of which build dictionary</param>
/// <returns> map of chars to their bitset</returns>

namespace huffman {
	CharBitMap buildDictionary(std::istream& is)
	{
		Node<Frequency> root = buildTree(getFrequency(is));
		CharBitMap dict = helperBuildDictionary(std::make_shared<Node<Frequency>>(root), 0);

		return dict;
	}
}

namespace {
	static constexpr std::size_t CHUNK_SIZE = 1024;

	/// <summary>
	/// Get frequency of every char in istream <para/>
	/// Returns pair with string instead of char, because more convinient to use in buildTree functon
	/// </summary>
	/// <returns>frequency map of every char.</returns>
	std::unordered_map < std::string, freq_t > getFrequency(std::istream& is)
	{
		std::unordered_map < std::string, freq_t > freq;
		char buffer[CHUNK_SIZE] = { 0 };
		std::string ch;
		while (is.read(buffer, CHUNK_SIZE) || is.gcount() > 0) {
			std::size_t bytesRead = is.gcount();
			for (size_t i = 0; i < bytesRead; i++)
			{
				ch = buffer[i];
				++freq[ch];
			}
		}
		return freq;
	}

	/// <summary>
	/// builds huffman tree
	/// </summary>
	/// <param name="char_frequency">- used chars and their frequency</param>
	/// <returns>top of huffman tree</returns>
	Node<Frequency> buildTree(std::unordered_map<std::string, freq_t> char_frequency)
	{

		std::vector<Node<Frequency>> nodes;
		for (const auto& pair : char_frequency)
		{
			assert(pair.first.size() == 1);	// initial frequency is supposed to be 
			// frequency of chars. Its std::string
			// initially for convinience (Mostly my inability to think about how to make it other way)
			nodes.emplace_back(Node<Frequency>(Frequency(pair)));
		}
		auto comp = [](const Node<Frequency> left, const Node<Frequency> right) {
			return left.getValue().freq > right.getValue().freq;
			};
		std::priority_queue<Node<Frequency>, std::vector<Node<Frequency>>, decltype(comp)>
			queue(nodes.begin(), nodes.end(), comp);

		while (queue.size() > 1) {
			Node<Frequency> first = queue.top();
			queue.pop();
			Node<Frequency> second = queue.top();
			queue.pop();

			Frequency first_freq = first.getValue();
			Frequency second_freq = second.getValue();

			std::string letters = first_freq.str + second_freq.str;
			freq_t new_freq = first_freq.freq + second_freq.freq;
			Node<Frequency> top(Frequency(letters, new_freq));
			Node<Frequency> branch(top);
			branch.setLeft(first);
			branch.setRight(second);
			queue.push(branch);
		}

		return queue.top();
	}

	CharBitMap helperBuildDictionary(std::shared_ptr<const Node<Frequency>> cur_node, std::bitset<BITSET_SIZE> cur_bits)
	{
		if (cur_node->isLeaf()) {
			Frequency cur = cur_node->getValue();
			assert(cur.str.size() == 1);// leaf node is always supposed to be single char. That just how huffman works
			return { {cur.str[0],cur_bits} };
		}

		CharBitMap dict;
		cur_bits <<= 1;
		auto left = cur_node->getLeft();
		auto right = cur_node->getRight();
		if (left != nullptr) {
			dict.merge(helperBuildDictionary(left, cur_bits));
			cur_bits.flip(0);
		}
		if (right != nullptr) {
			dict.merge(helperBuildDictionary(right, cur_bits));
		}

		return dict;
	}
}
