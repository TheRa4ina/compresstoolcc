#include <src/huffman.h>
#include <src/node.h>
#include <algorithm>
#include <queue>
#include <cassert>
#include <istream>


namespace {
	Node<Frequency> buildTree(CharFreqMap char_frequency);
		void getWidthDictionary(const Node<Frequency>& cur_node, Bits cur_bits, WidthDict& width_dict);
		std::array<uint8_t, UINT8_MAX> getWidthSortedAlphabet(const WidthDict& width_map);
}
// is calling them all build* even a right naming?
namespace huffman {

	WidthDict buildWidthDictionary(const CharFreqMap& frequency_map)
	{
		auto root = buildTree(frequency_map);	 
		WidthDict width_map;
		width_map.fill(width_map.size());
		if (root.getValue().freq != 0) {//if our tree is not empty
			getWidthDictionary(root,Bits(),width_map);
		}
		return width_map;
	}	

	CharBitMap buildDictionary(const WidthDict& width_map)
	{ // FIXME Calling it dictionary while it returns "map" seems ?weird?

		WidthDict width_sorted_alphabet = getWidthSortedAlphabet(width_map);
		CharBitMap dict;
		dict.fill(Bits(UINT32_MAX));
		Bits cur_code;
		uint8_t least_width_char= width_sorted_alphabet[0];
		//First element is all zeroes with original width
		cur_code <<= width_map[least_width_char];
		dict[least_width_char] = cur_code;
		Width last_width = cur_code.getWidth();
		for (size_t i = 1; i < width_sorted_alphabet.size(); i++)
		{
			uint8_t ch = width_sorted_alphabet[i];
			Width current_width = width_map[ch];

			++cur_code;
			if (last_width<current_width) {
				cur_code <<= current_width - cur_code.getWidth();
			}
			dict[ch] = cur_code;
			last_width = current_width;
		}
		return dict;
	}
	CharBitMap buildDictionary(const CharFreqMap& frequency_map)
	{
		return buildDictionary(buildWidthDictionary(frequency_map));
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
		for (size_t i=0;i<UINT8_MAX;i++)
		{
			FreqType char_freq = char_frequency[i];
			if (!char_freq == 0) {
				nodes.emplace_back(Node<Frequency>(Frequency(i,char_frequency[i])));
			}
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

	
	// TODO wrap it, so it returns width dict, but like, it can still use this approach,
	// basically WidthDict getWidthDictionary(cur_node,cur_code)
	void getWidthDictionary(const Node<Frequency>& cur_node, Bits cur_code,WidthDict& width_dict)
	{
		if (cur_node.isLeaf()) {
			Frequency cur = cur_node.getValue();

			// https://developercommunity.visualstudio.com/t/c28020-false-positives/923103
			#pragma warning(suppress: 28020)//size of dict is 256. uint8_t will never be >=256
			width_dict[cur.str] = cur_code.getWidth();
		}
		cur_code <<= 1;
		auto left = cur_node.getLeft();
		auto right = cur_node.getRight();
		if (left != nullptr) {
			(getWidthDictionary(*left, cur_code, width_dict));
			++cur_code;
		}
		if (right != nullptr) {
			(getWidthDictionary(*right, cur_code, width_dict));
		}
	}
	std::array<uint8_t, UINT8_MAX> getWidthSortedAlphabet(const WidthDict& width_map)
	{
		std::array<uint8_t, UINT8_MAX> alphabet;
		for (size_t i = 0; i < alphabet.size(); i++)
		{
			alphabet[i] = i;
		}
		// Sort alphabet by width of char
		std::sort(alphabet.begin(), alphabet.end(),
			[&width_map](const auto& left, const auto& right) {
				if (width_map[left] == width_map[right]) {
					return left < right;
				}
				else {
					return width_map[left] < width_map[right];
				}
			});
		return alphabet;
	}
}
