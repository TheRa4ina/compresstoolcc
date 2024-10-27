#include "src/util.h"

static constexpr size_t CHUNK_SIZE = 1024;
CharFreqMap getFrequency(std::istream& is)
{
	CharFreqMap freq{};
	char buffer[CHUNK_SIZE] = { 0 };
	uint8_t ch;
	while (is.read(buffer, CHUNK_SIZE) || is.gcount() > 0) {
		std::size_t bytesRead = is.gcount();
		for (size_t i = 0; i < bytesRead; i++)
		{
			ch = reinterpret_cast<uint8_t&>(buffer[i]);
			++freq[ch];
		}
	}
	return freq;
}