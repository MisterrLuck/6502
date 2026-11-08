#include <mos_6502.hpp>

#include <iostream>
#include <format>
using std::cout, std::format;

MOS6502::MOS6502()
{
	S = 0xFF;

	P = 0;

	A = 0;
	X = 0;
	Y = 0;
}

uint8_t MOS6502::fetch()
{
	uint8_t byte = memory[PC++];
	// PC += 2;
	return byte;
}

uint16_t MOS6502::fetchWord()
{
	return fetch() | (fetch() << 8);
}

// HACK: Idk if having the bool is fine
uint16_t MOS6502::readWord(uint16_t address, bool zero_page)
{
	if (zero_page)
		return memory[address & 0xFF] | (memory[(address+1) & 0xFF] << 8);
	return memory[address] | (memory[address+1] << 8);
}


// Address Modes
uint8_t MOS6502::indexZeroPage(uint8_t address, uint8_t index)
{
	return (address + index) & 0xFF;
}

uint16_t MOS6502::indexAbsolute(uint16_t address, uint8_t offset)
{
	return (address + offset) & 0xFFFF;
}

uint16_t MOS6502::xIndirect(uint8_t address)
{
	address = indexZeroPage(address, X);
	return readWord(address, true);
}

uint16_t MOS6502::yIndirect(uint8_t address)
{
	uint16_t ptr = readWord(address, true);
	return indexAbsolute(ptr, Y);
}


// Flag Functions
void MOS6502::updateNegativeFlag(uint8_t value)
{
	if (value >> 7)
		setFlag(Flag::Negative);
	else
		clearFlag(Flag::Negative);
	
}

void MOS6502::updateZeroFlag(uint8_t value)
{
	if (value == 0)
		setFlag(Flag::Zero);
	else
		clearFlag(Flag::Zero);
}



void MOS6502::setFlag(Flag f)
{
	uint8_t flag_bit = (0b1 << (int) f) & 0xFF;
	uint8_t mask = (~(0b1 << (int) f)) & 0xFF;

	P = (P & mask) | flag_bit;
}

void MOS6502::clearFlag(Flag f)
{
	uint8_t mask = (~(0b1 << (int) f)) & 0xFF;

	P &= mask;
}
