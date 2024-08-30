#include <src/compressor.h>
#include <bit>
#include <fstream>


// TODO Canonical huffman code
typedef uint8_t Byte;

void Compressor::compress(std::istream & input_stream, std::ostream & output_stream)
{
	CharFreqMap frequency_map = getFrequency(input_stream);
	dictionary = huffman::buildDictionary(frequency_map);
	input_stream.clear();
	input_stream.seekg(0, input_stream.beg);

	writeHeader(output_stream);
	writeCompressed(input_stream,output_stream);
}

/// <summary>
/// Get frequency of every char in istream <para/>
/// Returns pair with string instead of char, because more convinient to use in buildTree functon
/// </summary>
/// <returns>frequency map of every char.</returns>
CharFreqMap Compressor::getFrequency(std::istream& is)
{
	CharFreqMap freq;
	char buffer[CHUNK_SIZE] = { 0 };
	char ch;
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

void Compressor::writeHeader(std::ostream& os)
{
	os.put(0);//placeholder for unused bits, no idea how to make better currently
	for (const auto& [ch, val] : dictionary) {
		os.put(ch);
		os.write(reinterpret_cast<const char*>(&val.bits), sizeof val.bits);
	}
}

void Compressor::writeCompressed(std::istream& is, std::ostream& os)
{
	constexpr size_t BITS_IN_BYTE = 8;
	char buffer[CHUNK_SIZE] = { 0 };

	Bits cur_bits(0,0);
	char ch = '\0';
	while (is.read(buffer, CHUNK_SIZE) || is.gcount() > 0) {
		std::size_t bytes_read = is.gcount();
		for (size_t i =0;i<bytes_read;i++)
		{
			ch = buffer[i];
			Bits char_bits  = dictionary[ch];
			uint8_t bits_width = char_bits.width;// using max, because 
																	// std::bit_width(0b0)==0
																	
			// keeping track of bit_width by additioning instead of just using std::bit_width
			// on current because bits mapped to char could start with 0
			cur_bits  <<= bits_width;
			cur_bits  |=  char_bits.bits;

			while (cur_bits.width >= BITS_IN_BYTE) {
				uint8_t cut_off = cur_bits.width - BITS_IN_BYTE;

				// push off remainder width(cur_width) and save lower 8 bits to Byte b
				//Was		| 0000 0011 | 1010 0110 |
				//Shifted to| 0000 0000 | 1110 1001 | (10 was cutoff)
				//b contains| 1110 1001 |
				Byte b = ((cur_bits >> (cut_off)) & 0xff).bits;
				os.put(b);

				// Remember only remainder of bits, since last one
				// Was		|0000 0011|1010 0110|
				// Mask		|0000 0000|0000 0011|
				// Became	|0000 0000|0000 0010|
				cur_bits &= ~(~0U << cut_off);
				//		   \_______Mask_______/
				cur_bits.width = cut_off;
			}
		}
	}
	Byte b = cur_bits.bits;
	os.put(b);

	// write out unused bits in a placeholder
	os.seekp(0);
	uint8_t unused_bits_amount = BITS_IN_BYTE - cur_bits.width;
	os.put(unused_bits_amount);
}
