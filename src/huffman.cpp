#include "src/huffman.h"
#include <algorithm>
#include <queue>
#include <vector>
#include <cassert>

namespace {
	Node<Frequency> buildTree(CharFreqMap char_frequency);
	CharBitMap helperBuildDictionary(const Node<Frequency>& cur_node, Bits cur_bits);
}

namespace huffman {
/// <summary>
/// Builds Huffman dictionary from istream
/// </summary>
/// <param name="is">input istream, of which build dictionary</param>
/// <returns> map of chars to their bitset</returns>
	CharBitMap buildDictionary(CharFreqMap& frequency_map)
	{
		auto root = buildTree(frequency_map);
		CharBitMap dict;
		if (root.getValue().freq != 0) {//if our tree is not empty
			dict = helperBuildDictionary(root, 0);
		}

		return dict;
	}
}

namespace {

	/// <summary>
	/// builds huffman tree
	/// </summary>
	/// <param name="char_frequency">- used chars and their frequency</param>
	/// <returns>top of huffman tree</returns>
	Node<Frequency> buildTree(CharFreqMap char_frequency)
	{
		if (char_frequency.size() == 0) {
			return {};
		}
		std::vector<Node<Frequency>> nodes;
		for (const auto& pair : char_frequency)
		{
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

			FreqType new_freq = first_freq.freq + second_freq.freq;
			Node<Frequency> top(Frequency('\0', new_freq));// using '\0' as unsed char
			Node<Frequency> branch(top);
			branch.setLeft(first);
			branch.setRight(second);
			queue.push(branch);
		}

		return queue.top();
	}

	CharBitMap helperBuildDictionary(const Node<Frequency>& cur_node, Bits cur_bits)
	{
		if (cur_node.isLeaf()) {
			Frequency cur = cur_node.getValue();
			return { {cur.str,cur_bits} };
		}

		CharBitMap dict;
		cur_bits <<= 1;
		auto left = cur_node.getLeft();
		auto right = cur_node.getRight();
		if (left != nullptr) {
			dict.merge(helperBuildDictionary(*left, cur_bits));
			cur_bits++;
		}
		if (right != nullptr) {
			dict.merge(helperBuildDictionary(*right, cur_bits));
		}

		return dict;
	}
}
