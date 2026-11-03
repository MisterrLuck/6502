#pragma once
#include <cstdint>

#define MEM_SIZE 65536
#define STACK_ADDRESS_OFFSET 0x0100

class MOS6502
{
public:
	MOS6502();

private:
	// A -> Accumulator: Arithmatic and Logical output
	// X, Y -> General Use: X Can get a copy of the stack pointer
	// PC -> Program Counter
	// S -> Stack Pointer
	// P -> Status Register / Flags
		// Negative: If the last operation had bit 7 set to one
		// Overflow: Something about adding two numbers causing an invalid 2's complement; ie `64 + 64 => -128`
		// -
		// Break Command: Set when the BRK instruction has been executed
		// Decimal Mode: Manually set, but will use Binary Coded Decimal during addition and subtraction
		// Interrupt Disable: Manually set, but will prevent responding to interrupts while active
		// Zero: If the last operation was zero
		// Carry: Set if last operation caused an overflow or underflow

	uint8_t X, Y, A, S, P;
	uint16_t PC;

	uint8_t memory[MEM_SIZE];

	// Zero Page:		0000 - 00FF
	// System Stack:	0100 - 01FF
	// Reserved:		FFFA - FFFF
	
	// Stack pointer decrements down the stack
};
