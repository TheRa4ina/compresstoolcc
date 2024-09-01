#include "src/bits.h"

Bits::Bits()
	: bits(0), width(0) {}

Bits::Bits(const uint32_t value)
	: bits(value), width(std::bit_width(bits)) {}

Bits::Bits(const uint32_t value, uint8_t width)
	: bits(value), width(width) {}

const uint32_t Bits::getBits() const
{
	return bits;
}

void Bits::setBits(uint32_t new_bits)
{
	bits = new_bits;
}

void Bits::setBits(uint32_t new_bits, uint8_t new_width)
{
	bits = new_bits;
	setWidth(new_width);
}

const uint8_t Bits::getWidth() const
{
	return width;
}

void Bits::setWidth(uint8_t new_width)
{
	// Remember only remainder of bits, since last one
	// Was		|0000 0011|1010 0110|
	// Mask		|0000 0000|0000 0011|
	// Became	|0000 0000|0000 0010|
	bits &= ~(~0U << new_width);
	//		\_______Mask_______/
	width = new_width;
}

Bits& Bits::operator++()
{
	++bits;
	if (std::has_single_bit(bits) && bits != 1) {
		++width;
	}
	return *this;
}

Bits Bits::operator++(int)
{
	Bits old = *this;
	operator++();
	return old;
}
Bits operator<<(Bits i, uint32_t shift)
{
	i.bits = i.bits << shift;
	i.width += shift;
	return i;
}

Bits operator>>(Bits i, uint32_t shift)
{
	i.bits = i.bits >> shift;
	i.width -= shift;
	return i;
}

Bits operator|(Bits i, uint32_t mask)
{
	i.bits = i.bits | mask;
	return i;
}
Bits operator|(Bits i, Bits mask)
{
	i.bits = i.bits | mask.bits;
	return i;
}
Bits operator&(Bits i, uint32_t mask)
{
	i.bits = i.bits & mask;
	return i;
}
Bits operator&(Bits i, Bits mask)
{
	i.bits = i.bits & mask.bits;
	return i;
}



