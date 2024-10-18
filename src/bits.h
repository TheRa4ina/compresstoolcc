#pragma once
#include <unordered_map>


class Bits {
public:
	Bits();
	Bits(const uint32_t value);
	Bits(const uint32_t value, uint8_t width);

	const uint32_t getBits() const;
	void setBits(uint32_t new_bits);
	void setBits(uint32_t new_bits, uint8_t new_width);
	
	/// <summary>
	/// Gets smallest number of bits needed to represent the current value<para/>
	/// If current value is zero, then returns 1
	/// </summary>
	/// <returns>one, if current value zero, otherwise smallest number of bits to represent value</returns>
	const uint8_t getWidth() const;
	/// <summary>
	/// zeroes out higher bits, leaving only new_width of lower bits
	/// </summary>
	/// <param name="new_width">amount of lower bits to stay</param>
	void setWidth(uint8_t new_width);

	Bits& operator++();
	Bits operator++(int);
	friend Bits operator| (Bits i, uint32_t mask);
	friend Bits operator| (Bits i, Bits mask);
	friend Bits operator& (Bits i, uint32_t mask);
	friend Bits operator& (Bits i, Bits mask);
	friend Bits operator<< (Bits i, uint32_t shift);
	friend Bits operator>> (Bits i, uint32_t shift);

	template <class T>
	Bits& operator<<=	(T shift) {
		*this = *this << shift;
		return *this;
	}
	template <class T>
	Bits& operator>>=(T shift)
	{
		*this = *this >> shift;
		return *this;
	}
	template <class T>
	Bits& operator|=(T mask)
	{
		*this = *this | mask;
		return *this;
	}
	template <class T>
	Bits& operator&=(T mask)
	{
		*this = *this & mask;
		return *this;
	}

private:
	uint8_t width;
	uint32_t bits;
};


namespace std {
	template <>
	struct hash<Bits> {
		size_t operator()(const Bits& b) const noexcept {
			// Combine the hashes of bits and width
			size_t h1 = std::hash<uint32_t>{}(b.getBits());
			size_t h2 = std::hash<uint8_t>{}(b.getWidth());
			return h1 ^ (h2 << 1); // or use any other hash combining method
		}
	};
}