#include <src/decompressor.h>
#include <src/huffman.h>
#include <unordered_map>
#include <format>
constexpr char END_OF_HEADER = '|';//For now it sucks maybe some later i'll do like
//config file or some idk

void Decompressor::decompress(std::istream& input_stream, std::ostream& output_stream)
{
	readHeader(input_stream);
	CharBitMap char_bits_map = huffman::buildDictionary(char_widths);
	dictionary = reverseCharBitMap(char_bits_map);
	writeUncompressed(input_stream, output_stream);
}

void Decompressor::readHeader(std::istream& input_stream)
{
	getUnsedBits(input_stream);
	getCharWidths(input_stream);
	uint8_t header_end= input_stream.get();//Gets END_OF_HEADER
	if (header_end != END_OF_HEADER) {
		throw std::runtime_error(std::format(
			"Header read failed.Expected end of header '{}', got '{}' instead"
			,END_OF_HEADER, header_end));
	}
}

void Decompressor::getUnsedBits(std::istream& input_stream)
{
	unused_bits = input_stream.get();
}

void Decompressor::getCharWidths(std::istream& input_stream)
{
	for (size_t i = 0; i < char_widths.size(); i++)
	{
		char_widths[i] = input_stream.get();
	}
}

BitCharMap Decompressor::reverseCharBitMap(CharBitMap dict)
{
	BitCharMap reverse_dict;
	for (size_t i = 0; i<dict.size();i++)
	{
		uint8_t ch = i;
		Bits ch_code = dict[i];
		reverse_dict[ch_code] = ch;
	}
	return reverse_dict;
}

void Decompressor::writeUncompressed(std::istream& is, std::ostream& os)
{
	constexpr size_t BITS_IN_BYTE = 8;
	char buffer[CHUNK_SIZE] = { 0 };

	Bits cur_bits;
	while (is.read(buffer, CHUNK_SIZE) || is.gcount() > 0) {
		std::size_t bytes_read = is.gcount();
		for (size_t i = 0; i < bytes_read; i++)
		{
			Byte b = buffer[i];
			//if last byte in file, dont read highest unused bits
			uint8_t bits_to_read = 8;
			if (is.eof() && i == (bytes_read - 1)) {
				b <<= unused_bits;
				bits_to_read -= unused_bits;
			}
			Byte mask = 0b10000000;
			for (size_t j = 0; j < bits_to_read; j++)
			{
				cur_bits <<= 1;
				uint8_t highest_bit = b & mask;
				if (highest_bit) {//if highest bit was 1;
					cur_bits++;
				}
				if (dictionary.contains(cur_bits)) {
					os.put(dictionary[cur_bits]);
					cur_bits.setWidth(0);
				}
				b <<= 1;
			}
		}
	}
}
