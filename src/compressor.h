#pragma once
#include<string>
#include <src/huffman.h>

class Compressor {
public:
	void compress(std::istream & input_stream, std::ostream & output_stream);
private:
	//TODO make frequency use char and not string
	CharFreqMap getFrequency(std::istream& is);

	void writeHeader(std::ostream& output_stream);
	void writeCompressed(std::istream& input_stream, std::ostream& output_stream);
	static constexpr size_t CHUNK_SIZE = 1024;
	CharBitMap dictionary;
};