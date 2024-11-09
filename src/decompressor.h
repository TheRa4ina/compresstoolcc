#pragma once
#include<src/huffman.h>
using BitCharMap = std::unordered_map<Bits, uint8_t>;
class Decompressor {
public:
	/// <summary>
	/// Writes to output_stream decompressed version of data from input_stream 
	/// </summary>
	void decompress(std::istream& input_stream, std::ostream& output_stream);
private:

	/// <summary>
	/// gets unused bits
	/// also reads unused bits
	/// </summary>
	/// <param name="input_stream">input stream at start of a header</param>
	/// <returns>code widths of each char</returns>
	void readHeader(std::istream& input_stream);
	void getUnsedBits(std::istream& input_stream);
	void getCharWidths(std::istream& input_stream);

	BitCharMap reverseCharBitMap(CharBitMap char_bits_map);

	void writeUncompressed(std::istream& input_stream, std::ostream& output_stream);
	static constexpr size_t CHUNK_SIZE = 1024;
	BitCharMap dictionary;
	WidthDict char_widths;
	uint8_t unused_bits = 0;
};