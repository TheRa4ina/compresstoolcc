#include "src/util.h"

static constexpr size_t CHUNK_SIZE = 1024;
CharFreqMap getFrequency(std::istream& is)
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