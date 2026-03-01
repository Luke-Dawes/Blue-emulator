#include "CPU.h"
#include <iterator>

int main() {
	CPU myCPU;

	uint16_t program0[6] = {
	0xF000, // Instruction #0
	0xF003,
	0xA004, // Jump to instruction #4 (fifth one)
	0xF005, // This shouldn't be executed
	0xF010,
	0xA000 // Jump back to instruction #0
	};


	size_t size = std::size(program0);

	myCPU.runProgram(program0, size);
}