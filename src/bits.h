#pragma once
#include <unordered_map>
#include <compare>

// would Width definition be expectable to find here?
typedef uint8_t Width;

class Bits {
public:
	/// <summary>
	/// Creates Bits with value=0 and width=0
	/// </summary>
	Bits();
	/// <summary>
	/// Creates Bits with value and width that minimally represents this value
	/// if value==0, width=1
	/// </summary>
	Bits(const uint32_t value);
	Bits(const uint32_t value, uint8_t width);

	const uint32_t getBits() const;
	void setBits(uint32_t new_bits);
	void setBits(uint32_t new_bits, uint8_t new_width);
	
	/// <returns>return width member value</returns>
	const uint8_t getWidth() const;
	/// <summary>
	/// zeroes out higher bits, leaving only new_width of lower bits
	/// </summary>
	/// <param name="new_width">amount of lower bits to stay</param>
	void setWidth(uint8_t new_width);

	Bits& operator++();
	Bits operator++(int);
	constexpr friend bool operator==(Bits const& lhs, Bits const& rhs)= default;
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
	uint32_t bits;
	Width width;
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