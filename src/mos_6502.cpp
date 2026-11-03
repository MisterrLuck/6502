#include "mos_6502.hpp"

MOS6502::MOS6502()
{
	S = 0xFF;

	P = 0;

	A = 0;
	X = 0;
	Y = 0;
}
