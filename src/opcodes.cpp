#include <iostream>
#include <format>

#include <mos_6502.hpp>
using std::cout, std::format;

void MOS6502::cycle()
{
	uint8_t opcode = fetch();
	int num_cycles = 0;

	// TODO: Change cycle based on the +p
	switch (opcode & 0xF0)
	{
		case 0x10:	// 0x1X
		{
			switch(opcode & 0x0F)
			{
				case 0x08:	// 0x18 : CLC : 1 byte, 2 cycles
				{
					num_cycles = 2;
					clearFlag(Flag::Carry);
				}
				break;
			}
		}
		break;
		case 0x30:	// 0x3X
		{
			switch(opcode & 0x0F)
			{
				case 0x08:	// 0x38 : SEC : 1 byte, 2 cycles
				{
					num_cycles = 2;
					setFlag(Flag::Carry);
				}
				break;
			}
		}
		break;
		case 0x50:	// 0x5X
		{
			switch(opcode & 0x0F)
			{
				case 0x08:	// 0x58 : CLI : 1 byte, 2 cycles
				{
					num_cycles = 2;
					clearFlag(Flag::Interrupt);
				}
				break;
			}
		}
		break;
		case 0x70:	// 0x7X
		{
			switch(opcode & 0x0F)
			{
				case 0x08:	// 0x78 : SEI : 1 byte, 2 cycles
				{
					num_cycles = 2;
					setFlag(Flag::Interrupt);
				}
				break;
			}
		}
		break;
		case 0x80:	// 0x8X
		{
			switch (opcode & 0x0F)
			{
				case 0x01:	// 0x81 : STA ($nn, X) : 2 bytes, 6 cycles
				{
					num_cycles = 6;
					uint16_t address = xIndirect(fetch());
					memory[address] = A;
				}
				break;
				case 0x04:	// 0x84 : STY $nn : 2 bytes, 3 cycles
				{
					num_cycles = 3;
					uint8_t address = fetch();
					memory[address] = Y;
				}
				break;
				case 0x05:	// 0x85 : STA $nn : 2 bytes, 3 cycles
				{
					num_cycles = 3;
					uint8_t address = fetch();
					memory[address] = A;
				}
				break;
				case 0x06:	// 0x86 : STX $nn : 2 bytes, 3 cycles
				{
					num_cycles = 3;
					uint8_t address = fetch();
					memory[address] = X;
				}
				break;
				case 0x08:	// 0x88 : DEY : 1 byte, 2 cycles
				{
					num_cycles = 2;
					Y--;

					updateZeroFlag(Y);
					updateNegativeFlag(Y);
				}
				break;
				case 0x0C:	// 0x8C : STY $nnnn : 3 bytes, 4 cycles
				{
					num_cycles = 4;
					uint16_t address = fetchWord();
					memory[address] = Y;
				}
				break;
				case 0x0D:	// 0x8D : STA $nnnn : 3 bytes, 4 cycles
				{
					num_cycles = 4;
					uint16_t address = fetchWord();
					memory[address] = A;
				}
				break;
				case 0x0E:	// 0x8E : STX $nnnn : 3 bytes, 4 cycles
				{
					num_cycles = 4;
					uint16_t address = fetchWord();
					memory[address] = X;
				}
				break;
			}
		}
		break;
		case 0x90:	// 0x9X
		{
			switch (opcode & 0x0F)
			{
				case 0x01:	// 0x91 : STA ($nn), Y : 2 bytes, 6 cycles
				{
					num_cycles = 6;
					uint16_t address = yIndirect(fetch());
					memory[address] = A;
				}
				break;
				case 0x04:	// 0x94 : STY $nn, X : 2 bytes, 4 cycles
				{
					num_cycles = 4;
					uint8_t address = indexZeroPage(fetch(), X);
					memory[address] = Y;
				}
				break;
				case 0x05:	// 0x95 : STA $nn, X : 2 bytes, 4 cycles
				{
					num_cycles = 4;
					uint8_t address = indexZeroPage(fetch(), X);
					memory[address] = A;
				}
				break;
				case 0x06:	// 0x96 : STX $nn, Y : 2 bytes, 4 cycles
				{
					num_cycles = 4;
					uint8_t address = indexZeroPage(fetch(), Y);
					memory[address] = X;
				}
				break;
				case 0x09:	// 0x99 : STA $nnnn, Y : 3 bytes, 5 cycles
				{
					num_cycles = 5;
					uint16_t address = indexAbsolute(fetchWord(), Y);
					memory[address] = A;
				}
				break;
				case 0x0D:	// 0x9D : STA $nnnn, X : 3 bytes, 5 cycles
				{
					num_cycles = 5;
					uint16_t address = indexAbsolute(fetchWord(), X);
					memory[address] = A;
				}
				break;
			}
		}
		break;
		case 0xA0:	// 0xAX
		{
			switch (opcode & 0x0F)
			{
				case 0x00:	// 0xA0 : LDY #$nn : 2 bytes, 2 cycles
				{
					num_cycles = 2;
					Y = fetch();

					updateZeroFlag(Y);
					updateNegativeFlag(Y);
				}
				break;
				case 0x01:	// 0xA1 : LDA ($nn, X) : 2 bytes, 6 cycles
				{
					num_cycles = 6;
					uint16_t address = xIndirect(fetch());
					A = memory[address];

					updateZeroFlag(A);
					updateNegativeFlag(A);
				}
				break;
				case 0x02:	// 0xA2 : LDX #$nn : 2 bytes, 2 cycles
				{
					num_cycles = 2;
					X = fetch();

					updateZeroFlag(X);
					updateNegativeFlag(X);
				}
				break;
				case 0x04:	// 0xA4 : LDY $nn : 2 bytes, 3 cycles
				{
					num_cycles = 3;
					uint8_t address = fetch();
					Y = memory[address];

					updateZeroFlag(Y);
					updateNegativeFlag(Y);
				}
				break;
				case 0x05:	// 0xA5 : LDA $nn : 2 bytes, 3 cycles
				{
					num_cycles = 3;
					uint8_t address = fetch();
					A = memory[address];

					updateZeroFlag(A);
					updateNegativeFlag(A);
				}
				break;
				case 0x06:	// 0xA6 : LDX $nn : 2 bytes, 3 cycles
				{
					num_cycles = 3;
					uint8_t address = fetch();
					X = memory[address];

					updateZeroFlag(X);
					updateNegativeFlag(X);
				}
				break;
				case 0x09:	// 0xA9 : LDA #$nn : 2 bytes, 2 cycles
				{
					num_cycles = 2;
					A = fetch();

					updateZeroFlag(A);
					updateNegativeFlag(A);
				}
				break;
				case 0x0C:	// 0xAC : LDY $nnnn : 3 bytes, 4 cycles
				{
					num_cycles = 4;
					uint16_t address = fetchWord();
					Y = memory[address];

					updateZeroFlag(Y);
					updateNegativeFlag(Y);
				}
				break;
				case 0x0D:	// 0xAD : LDA $nnnn : 3 bytes, 4 cycles
				{
					num_cycles = 4;
					uint16_t address = fetchWord();
					A = memory[address];

					updateZeroFlag(A);
					updateNegativeFlag(A);
				}
				break;
				case 0x0E:	// 0xAE : LDX $nnnn : 3 bytes, 4 cycles
				{
					num_cycles = 4;
					uint16_t address = fetchWord();
					X = memory[address];

					updateZeroFlag(X);
					updateNegativeFlag(X);
				}
				break;
			}
		}
		break;
		case 0xB0:	// 0xBX
		{
			switch (opcode & 0x0F)
			{
				case 0x01:	// 0xB1 : LDA ($nn), Y : 2 bytes, 5+p cycles
				{
					num_cycles = 5;
					uint16_t address = yIndirect(fetch());
					A = memory[address];

					updateZeroFlag(A);
					updateNegativeFlag(A);
				}
				break;
				case 0x04:	// 0xB4 : LDY $nn, X : 2 bytes, 4 cycles
				{
					num_cycles = 4;
					uint8_t address = indexZeroPage(fetch(), X);
					Y = memory[address];

					updateZeroFlag(Y);
					updateNegativeFlag(Y);
				}
				break;
				case 0x05:	// 0xB5 : LDA $nn, X : 2 bytes, 4 cycles
				{
					num_cycles = 4;
					uint8_t address = indexZeroPage(fetch(), X);
					A = memory[address];

					updateZeroFlag(A);
					updateNegativeFlag(A);
				}
				break;
				case 0x06:	// 0xB6 : LDX $nn, Y : 2 bytes, 4 cycles
				{
					num_cycles = 4;
					uint8_t address = indexZeroPage(fetch(), Y);
					X = memory[address];

					updateZeroFlag(X);
					updateNegativeFlag(X);
				}
				break;
				case 0x08:	// 0xB8 : CLV : 1 byte, 2 cycles
				{
					num_cycles = 2;
					clearFlag(Flag::Overflow);
				}
				break;
				case 0x09:	// 0xB9 : LDA $nnnn, Y : 3 bytes, 4+p cycles
				{
					num_cycles = 4;
					uint16_t address = indexAbsolute(fetchWord(), Y);
					A = memory[address];

					updateZeroFlag(A);
					updateNegativeFlag(A);
				}
				break;
				case 0x0C:	// 0xBC : LDY $nnnn, X : 3 bytes, 4+p cycles
				{
					num_cycles = 4;
					uint16_t address = indexAbsolute(fetchWord(), X);
					Y = memory[address];

					updateZeroFlag(Y);
					updateNegativeFlag(Y);
				}
				break;
				case 0x0D:	// 0xBD : LDA $nnnn, X : 3 bytes, 4+p cycles
				{
					num_cycles = 4;
					uint16_t address = indexAbsolute(fetchWord(), X);
					A = memory[address];

					updateZeroFlag(A);
					updateNegativeFlag(A);
				}
				break;
				case 0x0E:	// 0xBE : LDX $nnnn, Y : 3 bytes, 4+p cycles
				{
					num_cycles = 4;
					uint16_t address = indexAbsolute(fetchWord(), Y);
					X = memory[address];

					updateZeroFlag(X);
					updateNegativeFlag(X);
				}
				break;
			}
		}
		break;
		case 0xC0:	// 0xCX
		{
			switch(opcode & 0x0F)
			{
				case 0x06:	// 0xC6 : DEC $nn : 2 bytes, 5 cycles
				{
					num_cycles = 5;
					uint8_t address = fetch();
					memory[address]--;

					updateZeroFlag(memory[address]);
					updateNegativeFlag(memory[address]);
				}
				break;
				case 0x08:	// 0xC8 : INY : 1 byte, 2 cycles
				{
					num_cycles = 2;
					Y++;

					updateZeroFlag(Y);
					updateNegativeFlag(Y);
				}
				break;
				case 0x0A:	// 0xCA : DEX : 1 byte, 2 cycles
				{
					num_cycles = 2;
					X--;

					updateZeroFlag(X);
					updateNegativeFlag(X);
				}
				break;
				case 0x0E:	// 0xCE : DEC $nnnn : 3 bytes, 6 cycles
				{
					num_cycles = 6;
					uint16_t address = fetchWord();
					memory[address]--;

					updateZeroFlag(memory[address]);
					updateNegativeFlag(memory[address]);
				}
				break;
			}
		}
		break;
		case 0xD0:	// 0xDX
		{
			switch(opcode & 0x0F)
			{
				case 0x06:	// 0xD6 : DEC $nn, X : 2 bytes, 6 cycles
				{
					num_cycles = 6;
					uint8_t address = indexZeroPage(fetch(), X);
					memory[address]--;

					updateZeroFlag(memory[address]);
					updateNegativeFlag(memory[address]);
				}
				break;
				case 0x08:	// 0xD8 : CLD : 1 byte, 2 cycles
				{
					// TODO: decimal mode flag is indeterminate after a RESET?
					num_cycles = 2;
					clearFlag(Flag::Decimal);
				}
				break;
				case 0x0E:	// 0xDE : DEC $nnnn, X : 3 bytes, 7 cycles
				{
					num_cycles = 7;
					uint16_t address = indexAbsolute(fetchWord(), X);
					memory[address]--;

					updateZeroFlag(memory[address]);
					updateNegativeFlag(memory[address]);
				}
				break;
			}
		}
		break;
		case 0xE0:	// 0xEX
		{
			switch(opcode & 0x0F)
			{
				case 0x06:	// 0xE6 : INC $nn : 2 bytes, 5 cycles
				{
					num_cycles = 5;
					uint8_t address = fetch();
					memory[address]++;

					updateZeroFlag(memory[address]);
					updateNegativeFlag(memory[address]);
				}
				break;
				case 0x08:	// 0xE8 : INX : 1 byte, 2 cycles
				{
					num_cycles = 2;
					X++;

					updateZeroFlag(X);
					updateNegativeFlag(X);
				}
				break;
				case 0x0A:	// 0xEA : NOP : 1 byte, 2 cycles
				{
					num_cycles = 2;
				}
				break;
				case 0x0E:	// 0xEE : INC $nnnn : 3 bytes, 6 cycles
				{
					num_cycles = 6;
					uint16_t address = fetchWord();
					memory[address]++;

					updateZeroFlag(memory[address]);
					updateNegativeFlag(memory[address]);
				}
				break;
			}
		}
		break;
		case 0xF0:	// 0xFX
		{
			switch(opcode & 0x0F)
			{
				case 0x06:	// 0xF6 : INC $nn, X : 2 bytes, 6 cycles
				{
					num_cycles = 6;
					uint8_t address = indexZeroPage(fetch(), X);
					memory[address]++;

					updateZeroFlag(memory[address]);
					updateNegativeFlag(memory[address]);
				}
				break;
				case 0x08:	// 0xF8 : SED : 1 byte, 2 cycles
				{
					// TODO: decimal mode flag is indeterminate after a RESET?
					num_cycles = 2;
					setFlag(Flag::Decimal);
				}
				break;
				case 0x0E:	// 0xFE : INC $nnnn, X : 3 bytes, 7 cycles
				{
					num_cycles = 7;
					uint16_t address = indexAbsolute(fetchWord(), X);
					memory[address]++;

					updateZeroFlag(memory[address]);
					updateNegativeFlag(memory[address]);
				}
				break;
			}
		}
		break;
	}
}
