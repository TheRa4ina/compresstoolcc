#include "src/huffman.h"
#include "src/util.h"
#include <algorithm>
#include <queue>
#include <vector>
#include <cassert>

namespace {
	Node<Frequency> buildTree(CharFreqMap char_frequency);
	CharBitMap helperBuildDictionary(const Node<Frequency>& cur_node, Bits cur_bits);
}

namespace huffman {
	CharBitMap buildDictionary(CharFreqMap& frequency_map)
	{
		auto root = buildTree(frequency_map);
		CharBitMap dict;
		if (root.getValue().freq != 0) {//if our tree is not empty
			dict = helperBuildDictionary(root,Bits());
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
			Node<Frequency> branch(Frequency('\0', new_freq));// using '\0' as unsed char

			branch.setLeft(first);
			branch.setRight(second);
			queue.push(branch);
		}

		return queue.top();
	}

	CharBitMap helperBuildDictionary(const Node<Frequency>& cur_node, Bits cur_code)
	{
		if (cur_node.isLeaf()) {
			Frequency cur = cur_node.getValue();
			return { {cur.str,cur_code} };
		}

		CharBitMap dict;
		cur_code <<= 1;
		auto left = cur_node.getLeft();
		auto right = cur_node.getRight();
		if (left != nullptr) {
			dict.merge(helperBuildDictionary(*left, cur_code));
			++cur_code;
		}
		if (right != nullptr) {
			dict.merge(helperBuildDictionary(*right, cur_code));
		}

		return dict;
	}
}
