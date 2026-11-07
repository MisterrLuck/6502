#include <mos_6502.hpp>

void MOS6502::cycle()
{
	uint8_t opcode = fetch();
	int num_cycles = 0;

	// TODO: Change cycle based on the +p
	switch (opcode & 0xF0)
	{
		case 0x80: // 0x8X
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
		case 0x90: // 0x9X
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
		case 0xA0:
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
		case 0xB0:
		{
			switch (opcode & 0x0F)
			{
				case 0x01:	// 0xB1 : LDA ($nn), Y : 2 bytes, 5+p cycles
				{
					num_cycles = 5;
					uint8_t address = yIndirect(fetch());
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
	}
}
