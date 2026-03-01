#include "CPU.h"

void CPU::emulateCycle() {

	while (clockPulse < 9) {

		processTick(clockPulse);
		clockPulse++;
	}
	clockPulse = 1;
	state = FETCH;

}

void CPU::processTick(uint8_t tick) {
	switch (tick)
	{
	case 1:
		if (state == FETCH) {
			set12BitRegister(PC, MAR);
		}
		break;

	case 2:
		if (state == FETCH) {
			PC = (PC + 1) & 0x0FFF;
		}
		break;

	case 3:
		if (state == FETCH) {
			set12BitRegister(0x00, MDR);
		}
		break;

	case 4:
		if (state == FETCH) {
			MDR = RAM[MAR];
			set12BitRegister(0x00, CIR);
		}
		break;

	case 5:
		if (state == FETCH) {
			CIR = MDR;
		}
		break;

	case 6:
		break;
	case 7:
		break;
	case 9:
		break;
	case 8:
		break;
	default:
		break;
	}

	uint8_t INS = getInstruction();
	(this->*instructionFunctions[INS])(tick);

}