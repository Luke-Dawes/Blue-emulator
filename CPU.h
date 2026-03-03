#pragma once
#include <cstdint>
#include <vector>

#define RAMLENGTH 4096 //of words
enum STATE { FETCH, EXECUTE };


class CPU {
	uint8_t clockPulse = 1;

	STATE state = FETCH;
	
	uint16_t RAM[RAMLENGTH]; //ram is an array of 

	typedef uint16_t Register;
	Register accumulator; //16 bit
	Register CIR; //16 bit
	Register MDR; //16 bits

	Register MAR; //12 bits
	Register PC = 0x00; //12 bit

	Register dataIn; //12 bit
	Register dataOut; //12 bit
	Register IoSelecter; //unknown
	Register SR; //control switch - input data manually unknown
	Register Z; //16 bit

	uint8_t getInstruction()
	{
		return (CIR >> 12) & 0x0F;

		//CIR holds 16 bits now, meaning that the opcode
		//is just the top 4 bits.
	}

	uint16_t getData()
	{
		return CIR & 0x0FFF;
		//this applies a mask ignoring the top 4 bits to
		//only return the oprand 
		//should immediately be put in the MDR
	}

	void set12BitRegister(uint16_t value, Register& reg) {
		reg = value & 0x0FFF; 
		//keep only the bottom 12 bits, simulating the 12 bit register
	}


	void doHLT(uint8_t tick) { return; }
	void doADD(uint8_t tick) { return; }
	void doXOR(uint8_t tick) { return; }
	void doAND(uint8_t tick) { return; }
	void doIOR(uint8_t tick) { return; }
	void doNOT(uint8_t tick) { return; }

	void doLDA(uint8_t tick) { 
		
		if (tick == 8 && state == FETCH) {
			state = EXECUTE;
			MAR = CIR;
		}
		else if (tick == 2) {
			accumulator = 0;
		}
		else if (tick == 3) {
			MDR = 0;
		}
		else if (tick == 5) {
			accumulator = MDR;
		}
		else if (tick == 8 && state == EXECUTE) {
			state == FETCH;
			MAR = PC;
		}
	}

	void doSTA(uint8_t tick) { return; }
	void doSRJ(uint8_t tick) { return; }
	void doJMA(uint8_t tick) { return; }

	void doJMP(uint8_t tick) { 
		if (tick == 6) {
			PC = 0;
		}
		else if (tick == 7) {
			set12BitRegister(CIR, PC);
		}
		else if (tick == 8) {
			set12BitRegister(PC, MAR);
		}
	}
	void doINP(uint8_t tick) { return; }
	void doOUT(uint8_t tick) { return; }
	void doRAL(uint8_t tick) { return; }
	void doCSA(uint8_t tick) { return; }
	void doNOP(uint8_t tick) { if (tick == 8) set12BitRegister(PC, MAR); };

	void dumpRegisters() const
	{
		printf("PC: %04x Acc: %04x CIR: %04x Z: %04x MAR: %04x MDR: %04x ioSelecter: %02x data in: %02x data out: %02x\n", PC, accumulator, CIR, Z, MAR, MDR, (IoSelecter & 0x00FF), (dataIn & 0x00FF), (dataOut & 0x00FF));
	}


	void processTick(uint8_t tick);
	
	std::vector<void (CPU::*)(uint8_t tick)> instructionFunctions;

	void emulateCycle();

public:
	
	CPU() {
		instructionFunctions = 
		{
		&CPU::doHLT,
		&CPU::doADD,
		&CPU::doXOR,
		&CPU::doAND,
		&CPU::doIOR,
		&CPU::doNOT,
		&CPU::doLDA,
		&CPU::doSTA,
		&CPU::doSRJ,
		&CPU::doJMA,
		&CPU::doJMP,
		&CPU::doINP,
		&CPU::doOUT,
		&CPU::doRAL,
		&CPU::doCSA,
		&CPU::doNOP
		};
	}

	void runProgram(const uint16_t* program, size_t programSize)
	{
		memset(RAM, 0x00, sizeof(RAM));
		size_t copySize = (programSize < RAMLENGTH) ? programSize : RAMLENGTH;

		memmove(RAM, program, (copySize * sizeof(uint16_t)));

		PC = 0;
		set12BitRegister(PC, MAR);
		for (;;) {
			emulateCycle();
			dumpRegisters();
		}
	}
};