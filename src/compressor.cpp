#include <src/compressor.h>
#include <src/util.h>
#include <fstream>
#include <climits>


constexpr char END_OF_HEADER = '|';

void Compressor::compress(std::istream & input_stream, std::ostream & output_stream)
{
	CharFreqMap frequency_map = getFrequency(input_stream);
	input_stream.clear();
	input_stream.seekg(0, input_stream.beg);
	WidthDict char_widths = huffman::buildWidthDictionary(frequency_map);
	writeHeader(output_stream,char_widths);
	dictionary = huffman::buildDictionary(char_widths);
	writeCompressed(input_stream,output_stream);
}

void Compressor::writeHeader(std::ostream& os, WidthDict& char_widths)
{
	//no idea how to make better
	os.put(0);//placeholder for unused bits to write onto from writeCompressed

	for (const Width width : char_widths)
	{
		os.put(width);
	}
	os.put(END_OF_HEADER);
}

void Compressor::writeCompressed(std::istream& is, std::ostream& os)
{
	constexpr size_t BITS_IN_BYTE = 8;
	char buffer[CHUNK_SIZE] = { 0 };

	Bits cur_bits;
	uint8_t ch = '\0';
	while (is.read(buffer, CHUNK_SIZE) || is.gcount() > 0) {
		std::size_t bytes_read = is.gcount();
		for (size_t i =0;i<bytes_read;i++)
		{
			ch = reinterpret_cast<uint8_t&>(buffer[i]);
			
			// https://developercommunity.visualstudio.com/t/c28020-false-positives/923103
			#pragma warning(suppress: 28020)//size of dict is 256. char will never be >=256
			Bits char_bits  = dictionary[ch];
			Width bits_width = char_bits.getWidth();
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
				// this 10 that was cut_off, now saved in cur_bits
				cur_bits.setWidth(cut_off);
			}
		}
	}
	Byte b = cur_bits.getBits();
	os.put(b);

	int end_pos = os.tellp();
	// write out unused bits in a placeholder
	os.seekp(0);
	uint8_t unused_bits_amount = BITS_IN_BYTE - cur_bits.getWidth();
	os.put(unused_bits_amount);
	//restore back to the end of ostream
	os.seekp(end_pos);
}
