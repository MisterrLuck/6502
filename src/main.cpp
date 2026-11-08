#include <iostream>
#include <format>

#include <mos_6502.hpp>
using std::cout, std::format;

int main()
{
	MOS6502 mos;

	cout << format("Status: {:08b}\n", mos.P);
	mos.setFlag(Flag::Negative);
	cout << format("Status: {:08b}\n", mos.P);
	mos.setFlag(Flag::Overflow);
	cout << format("Status: {:08b}\n", mos.P);
	mos.setFlag(Flag::Break);
	cout << format("Status: {:08b}\n", mos.P);
	mos.setFlag(Flag::Decimal);
	cout << format("Status: {:08b}\n", mos.P);
	mos.setFlag(Flag::Interrupt);
	cout << format("Status: {:08b}\n", mos.P);
	mos.setFlag(Flag::Zero);
	cout << format("Status: {:08b}\n", mos.P);
	mos.setFlag(Flag::Carry);
	cout << format("Status: {:08b}\n", mos.P);
	mos.clearFlag(Flag::Negative);
	cout << format("Status: {:08b}\n", mos.P);

	return 0;
}
