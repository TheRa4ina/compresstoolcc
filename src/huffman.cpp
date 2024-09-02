#include <src/huffman.h>
#include <src/node.h>
#include <algorithm>
#include <queue>
#include <cassert>
#include <istream>

namespace {
	Node<Frequency> buildTree(CharFreqMap char_frequency);
	void helperBuildDictionary(const Node<Frequency>& cur_node, Bits cur_bits, CharBitMap& dict);
}

namespace huffman {
	CharBitMap buildDictionary(CharFreqMap& frequency_map)
	{
		auto root = buildTree(frequency_map);
		CharBitMap dict;
		if (root.getValue().freq != 0) {//if our tree is not empty
			helperBuildDictionary(root,Bits(),dict);
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

	void helperBuildDictionary(const Node<Frequency>& cur_node, Bits cur_code,CharBitMap& dict)
	{
		if (cur_node.isLeaf()) {
			Frequency cur = cur_node.getValue();

			// https://developercommunity.visualstudio.com/t/c28020-false-positives/923103
			#pragma warning(suppress: 28020)//size of dict is 256. uint8_t will never be >=256
			dict[cur.str] = cur_code;
		}
		cur_code <<= 1;
		auto left = cur_node.getLeft();
		auto right = cur_node.getRight();
		if (left != nullptr) {
			(helperBuildDictionary(*left, cur_code,dict));
			++cur_code;
		}
		if (right != nullptr) {
			(helperBuildDictionary(*right, cur_code,dict));
		}
	}
}
