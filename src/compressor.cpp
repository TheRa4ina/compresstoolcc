#include <src/compressor.h>
#include <src/util.h>
#include <fstream>
#include <climits>

// TODO Canonical huffman code
typedef uint8_t Byte;
constexpr char END_OF_HEADER = '|';

void Compressor::compress(std::istream & input_stream, std::ostream & output_stream)
{
	CharFreqMap frequency_map = getFrequency(input_stream);
	dictionary = huffman::buildDictionary(frequency_map);
	input_stream.clear();
	input_stream.seekg(0, input_stream.beg);

	writeHeader(output_stream);
	writeCompressed(input_stream,output_stream);
}

void Compressor::writeHeader(std::ostream& os)
{

	os.put(0);//placeholder for unused bits, no idea how to make better currently
	for (size_t i = 0; i < dictionary.size(); i++)
	{
		Bits val = dictionary[i];
		os.put(char(i));
		int bits = val.getBits();
		os.write(reinterpret_cast<const char*>(&bits), sizeof bits);
	}
	os.put(END_OF_HEADER);
}

void Compressor::writeCompressed(std::istream& is, std::ostream& os)
{
	constexpr size_t BITS_IN_BYTE = CHAR_BIT;
	char buffer[CHUNK_SIZE] = { 0 };

	Bits cur_bits;
	char ch = '\0';
	while (is.read(buffer, CHUNK_SIZE) || is.gcount() > 0) {
		std::size_t bytes_read = is.gcount();
		for (size_t i =0;i<bytes_read;i++)
		{
			ch = buffer[i];
			// https://developercommunity.visualstudio.com/t/c28020-false-positives/923103
			#pragma warning(suppress: 28020)//size of dict is 256. char will never be >=256
			Bits char_bits  = dictionary[ch];
			uint8_t bits_width = char_bits.getWidth();// using max, because 
																	// std::bit_width(0b0)==0
																	
			// keeping track of bit_width by additioning instead of just using std::bit_width
			// on current because bits mapped to char could start with 0
			cur_bits  <<= bits_width;
			cur_bits  |=  char_bits.getBits();

			while (cur_bits.getWidth() >= BITS_IN_BYTE) {
				uint8_t cut_off = cur_bits.getWidth() - BITS_IN_BYTE;

				// push off remainder width(cur_width) and save lower 8 bits to Byte b
				//Was		| 0000 0011 | 1010 0110 |
				//Shifted to| 0000 0000 | 1110 1001 | (10 was cutoff)
				//b contains| 1110 1001 |
				Byte b = ((cur_bits >> (cut_off)) & 0xff).getBits();
				os.put(b);
				cur_bits.setWidth(cut_off);
			}
		}
	}
	Byte b = cur_bits.getBits();
	os.put(b);

	// write out unused bits in a placeholder
	os.seekp(0);
	uint8_t unused_bits_amount = BITS_IN_BYTE - cur_bits.getWidth();
	os.put(unused_bits_amount);
}
