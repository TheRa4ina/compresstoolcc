#pragma once
#include<string>
#include<src/huffman.h>

class Compressor {
public:
	/// <summary>
	/// Writes to output_stream compressed version of data from input_stream 
	/// </summary>
	void compress(std::istream & input_stream, std::ostream & output_stream);
private:

	void writeHeader(std::ostream& output_stream);
	void writeCompressed(std::istream& input_stream, std::ostream& output_stream);
	static constexpr size_t CHUNK_SIZE = 1024;
	CharBitMap dictionary;

};