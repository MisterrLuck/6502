#include <fstream>
#include <iostream>
#include <format>

#include <nlohmann/json.hpp>
// #include <nlohmann/json_literals.hpp>
// #include <nlohmann/json_fwd.hpp>
// #include <nlohmann/json_view.hpp>
#include <mos_6502.hpp>
using nlohmann::json, std::cout, std::cin, std::format;


void loadMemory(MOS6502 &mos, const json &mem);
int compareMemory(MOS6502 &mos, const json &mem);
void printTest(const json &test);


int main()
{
	cout << "Opcode to test: 0x";
	char _0, _1;
	cin >> _0;
	cin >> _1;

	std::ifstream file(format("../tests/sst/{}{}.json", _0, _1));
	json all_tests = json::parse(file);
	const json& test = all_tests[0];

	MOS6502 mos;

	printTest(test);

	loadMemory(mos, test["initial"]);
	if (compareMemory(mos, test["initial"]))
	{
		cout << "Didn't load properly\n";
		return -1;
	}

	mos.cycle();

	cout << "Compared to Target\n";
	int result = compareMemory(mos, test["final"]);

	if (!result)
		cout << "Run Successful\n";
	else
		cout << format("Error: Count {}\n", result);

	return 0;
}

void loadMemory(MOS6502 &mos, const json &mem)
{
	mos.PC = mem["pc"].get<uint16_t>();
	mos.S = mem["s"].get<uint8_t>();
	mos.A = mem["a"].get<uint8_t>();
	mos.X = mem["x"].get<uint8_t>();
	mos.Y = mem["y"].get<uint8_t>();
	mos.P = mem["p"].get<uint8_t>();

	for (const auto &i : mem["ram"])
		// mos.memory[static_cast<int>(i[0])] = static_cast<int>(i[1]);
		mos.memory[i[0].get<uint16_t>()] = i[1].get<uint8_t>();
}

int compareMemory(MOS6502 &mos, const json &mem)
{
	int count = 0;

	if (mos.PC != mem["pc"].get<uint16_t>())
	{
		count++;
		cout << "PC issue\n";
	}
	if (mos.S != mem["s"].get<uint8_t>())
	{
		cout << "S issue\n";
		count++;
	}
	if (mos.A != mem["a"].get<uint8_t>())
	{
		cout << "A issue\n";
		count++;
	}
	if (mos.X != mem["x"].get<uint8_t>())
	{
		cout << "X issue\n";
		count++;
	}
	if (mos.Y != mem["y"].get<uint8_t>())
	{
		cout << "Y issue\n";
		count++;
	}
	if (mos.P != mem["p"].get<uint8_t>())
	{
		cout << "P issue\n";
		count++;
	}

	for (const auto& i : mem["ram"])
	{
		uint16_t address = i[0].get<uint16_t>();
		uint8_t value = i[1].get<uint8_t>();
		if (mos.memory[address] != value)
		{
			cout << format("0x{:x} ({}): 0x{:x} != 0x{:x}, {} != {}\n", address, address, mos.memory[address], value, mos.memory[address], value);
			count++;
		}
	}

	return count;
}

void printTest(const json &test)
{
	cout << "Initial State\n";
	cout << test["initial"].dump(4) << "\n";
	cout << "Target State\n";
	cout << test["final"].dump(4) << "\n";
}
