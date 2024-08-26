#include <src/compressor.h>
#include <bit>
#include <fstream>

typedef uint16_t Bits;

/// <summary>
/// Writes to output_stream compressed version of data from input_stream 
/// </summary>
void Compressor::compress(std::istream & input_stream, std::ostream & output_stream)
{
	dictionary = huffman::buildDictionary(input_stream);
	input_stream.clear();
	input_stream.seekg(0, input_stream.beg);

	writeHeader(output_stream);
	writeCompressed(input_stream,output_stream);
}

void Compressor::writeHeader(std::ostream& os)
{
	os.put(0);//placeholder for unused bits, no idea how to make better currently
	for (const auto& [ch, val] : dictionary) {
		os << ch << val;
	}
}

void Compressor::writeCompressed(std::istream& is, std::ostream& os)
{
	constexpr size_t BITS_IN_BYTE = 8;
	constexpr size_t CHUNK_SIZE   = 1024;
	char buffer[CHUNK_SIZE] = { 0 };
	Bits cur_bits  = 0b0;
	uint8_t cur_width = 0;
	char ch = '\0';
	while (is.read(buffer, CHUNK_SIZE) || is.gcount() > 0) {
		std::size_t bytes_read = is.gcount();
		for (size_t i =0;i<bytes_read;i++)
		{
			ch = buffer[i];
			Bits char_bits  = dictionary[ch];
			Bits bits_width = std::max(1, std::bit_width(char_bits));// using max, because 
																	// std::bit_width(0b0)==0
																	
			// keeping track of bit_width by additioning instead of just using std::bit_width
			// on current because bits mapped to char could start with 0
			cur_width +=  bits_width;
			cur_bits  <<= bits_width;
			cur_bits  |=  char_bits;

			if (cur_width >= BITS_IN_BYTE) {
				uint8_t cut_off = cur_width - BITS_IN_BYTE;

				// push off remainder width(cur_width) and save lower 8 bits to Byte b
				//Was		| 0000 0011 | 1010 0110 |
				//Shifted to| 0000 0000 | 1110 1001 | (10 was cutoff)
				//b contains| 1111 1001 |
				Byte b = (cur_bits >> (cut_off)) & 0xff;
				os.put(b);

				// Remember only remainder of bits, since last one
				// Was		|0000 0011|1010 0110|
				// Mask		|0000 0000|0000 0011|
				// Became	|0000 0000|0000 0010|
				cur_bits &= ~(~0U << cut_off);
				//		   \_______Mask_______/
				cur_width = cut_off;
			}
		}
	}
	Byte b = cur_bits;
	os.put(b);

	// write out unused bits in a placeholder
	os.seekp(0);
	uint8_t unused_bits_amount = BITS_IN_BYTE - cur_width;
	os.put(unused_bits_amount);
}
