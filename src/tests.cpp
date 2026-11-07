#include <fstream>
#include <iostream>
#include <format>
#include <vector>

#include <nlohmann/json.hpp>
// #include <nlohmann/json_literals.hpp>
// #include <nlohmann/json_fwd.hpp>
// #include <nlohmann/json_view.hpp>
#include <mos_6502.hpp>
using nlohmann::json, std::cout, std::cin, std::format, std::vector;


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

	int count = 0;
	vector<int> bad_tests;

	for (const auto& test : all_tests)
	{
		MOS6502 mos;

		cout << format("Run {}\n", count);

		loadMemory(mos, test["initial"]);
		if (compareMemory(mos, test["initial"]))
			cout << "Didn't load properly\n";
		else
		{
			mos.cycle();

			cout << "Compared to Target\n";
			int result = compareMemory(mos, test["final"]);

			if (!result)
				cout << "Run Successful\n";
			else
			{
				bad_tests.push_back(count);
				cout << format("Error: Count {}\n", result);
			}

			count++;
		}
	}

	cout << format("\nResults from opcode 0x{}{} test\n", _0, _1);
	if (!bad_tests.size())
		cout << "Completely Successful Run\n";
	else
	{
		cout << format("{} tests failed. Do you wish to see all? (y/N) ", bad_tests.size());
		char answer;
		cin >> answer;

		if (answer == 'y')
		{
			for (int bad : bad_tests)
				cout << format("Test Number {} failed\n", bad);
		}
	}

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

	uint16_t target_pc = mem["pc"].get<uint16_t>();
	uint8_t target_s = mem["s"].get<uint8_t>();
	uint8_t target_a = mem["a"].get<uint8_t>();
	uint8_t target_x = mem["x"].get<uint8_t>();
	uint8_t target_y = mem["y"].get<uint8_t>();
	uint8_t target_p = mem["p"].get<uint8_t>();

	if (mos.PC != target_pc)
	{
		count++;
		cout << format("PC: 0x{:x} != 0x{:x}, {} != {}\n", mos.PC, target_pc, mos.PC, target_pc);
	}
	if (mos.S != target_s)
	{
		count++;
		cout << format("S: 0x{:x} != 0x{:x}, {} != {}\n", mos.S, target_s, mos.S, target_s);
	}
	if (mos.A != target_a)
	{
		count++;
		cout << format("A: 0x{:x} != 0x{:x}, {} != {}\n", mos.A, target_a, mos.A, target_a);
	}
	if (mos.X != target_x)
	{
		count++;
		cout << format("X: 0x{:x} != 0x{:x}, {} != {}\n", mos.X, target_x, mos.X, target_x);
	}
	if (mos.Y != target_y)
	{
		count++;
		cout << format("Y: 0x{:x} != 0x{:x}, {} != {}\n", mos.Y, target_y, mos.Y, target_y);
	}
	if (mos.P != target_p)
	{
		count++;
		cout << format("P: 0b{:08b} != 0b{:08b}, {} != {}\n", mos.P, target_p, mos.P, target_p);
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
