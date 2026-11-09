#include <fstream>
#include <iostream>
#include <format>
#include <vector>

#include <nlohmann/json.hpp>
#include <mos_6502.hpp>
using nlohmann::json, std::cout, std::cin, std::format, std::vector;


void loadMemory(MOS6502 &mos, const json &mem);
int compareMemory(MOS6502 &mos, const json &mem);
void printTest(const json &test);


int main()
{
	int opcode;
	MOS6502 mos;

	vector<int> bad_opcodes;

	for (opcode = 0; opcode <= 0xFF; opcode++)
	{
		std::ifstream file(format("../tests/sst/{:x}.json", opcode));
		if (!file.is_open())
			continue;

		json all_tests = json::parse(file);

		int count = 0;
		vector<int> bad_tests;

		for (const auto& test : all_tests)
		{
			// cout << format("Run {}\n", count);

			loadMemory(mos, test["initial"]);
			if (compareMemory(mos, test["initial"]))
			{
				cout << "Didn't load properly\n";
				continue;
			}

			mos.cycle();

			int result = compareMemory(mos, test["final"]);

			// if (!result)
				// cout << "Run Successful\n";
			if (result)
			{
				bad_tests.push_back(count);
				// cout << format("Error: Count {}\n", result);
			}

			count++;
		}

		// cout << format("\nResults from opcode 0x{}{} test\n", _0, _1);
		if (!bad_tests.size())
			cout << format("0x{:x}: Completely Successful Run\n", opcode);
		else
		{
			cout << format("0x{:x}: {} tests failed\n", opcode, bad_tests.size());
			bad_opcodes.push_back(opcode);
			// cout << format("{} tests failed. Do you wish to see all? (y/N) ", bad_tests.size());
			// char answer;
			// cin >> answer;
			//
			// if (answer == 'y')
			// {
			// 	int start = bad_tests[0];
			// 	int end = bad_tests[0]-1;
			// 	for (int bad : bad_tests)
			// 	{
			// 		if (bad == end+1)
			// 			end++;
			// 		else
			// 		{
			// 			cout << format("Range {}-{} failed\n", start, end);
			// 			start = bad;
			// 			end = bad;
			// 		}
			// 	}
			// 	if (end == bad_tests.size()-1)
			// 		cout << format("Range {}-{} failed\n", start, end);
			// }
		}
	}

	cout << "\nFinal Results\n";
	for (int bad_opcode : bad_opcodes)
	{
		cout << format("{:x}: Failed\n", bad_opcode);
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
		// cout << format("PC: 0x{:x} != 0x{:x}, {} != {}\n", mos.PC, target_pc, mos.PC, target_pc);
	}
	if (mos.S != target_s)
	{
		count++;
		// cout << format("S: 0x{:x} != 0x{:x}, {} != {}\n", mos.S, target_s, mos.S, target_s);
	}
	if (mos.A != target_a)
	{
		count++;
		// cout << format("A: 0x{:x} != 0x{:x}, {} != {}\n", mos.A, target_a, mos.A, target_a);
	}
	if (mos.X != target_x)
	{
		count++;
		// cout << format("X: 0x{:x} != 0x{:x}, {} != {}\n", mos.X, target_x, mos.X, target_x);
	}
	if (mos.Y != target_y)
	{
		count++;
		// cout << format("Y: 0x{:x} != 0x{:x}, {} != {}\n", mos.Y, target_y, mos.Y, target_y);
	}
	if (mos.P != target_p)
	{
		count++;
		// cout << format("P: 0b{:08b} !=\n   0b{:08b}, {} != {}\n", mos.P, target_p, mos.P, target_p);
	}

	for (const auto& i : mem["ram"])
	{
		uint16_t address = i[0].get<uint16_t>();
		uint8_t value = i[1].get<uint8_t>();
		if (mos.memory[address] != value)
		{
			// cout << format("0x{:x} ({}): 0x{:x} != 0x{:x}, {} != {}\n", address, address, mos.memory[address], value, mos.memory[address], value);
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
