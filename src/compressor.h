#pragma once
#include<string>
#include <src/huffman.h>

class Compressor {
public:
	void compress(std::istream & input_stream, std::ostream & output_stream);
private:
	void writeHeader(std::ostream& output_stream);
	void writeCompressed(std::istream& input_stream, std::ostream& output_stream);

	CharBitMap dictionary;
};