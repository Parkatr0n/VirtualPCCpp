
#include "CPU.h"

CPU::CPU(int cacheSize, Memory& ram_, GPU& gpu_)
	:
	registerOP1(0),
	registerOP2(0),
	register0(0),
	register1(0),
	programCounter(0),
	keyboardRegister(97),
	inputCount(0),
	firstAvailable(0),
	currentTime(0),
	ram(ram_),
	halt(false),
	cache(Memory(cacheSize)),
	gpu(gpu_){
}

enum codes {
	Halt = 0,
	LoadReg1Cache = 1,
	LoadReg2Cache = 2,
	LoadReg1Ram = 3,
	LoadReg2Ram = 4,
	ClearReg1 = 5,
	ClearReg2 = 6,
	WriteReg0ToCache = 7,
	WriteReg1ToCache = 8,
	WriteReg0ToRam = 9,
	WriteReg1ToRam = 10,
	WriteCacheToRam = 11,
	WriteRamToCache = 12,
	Sum = 20,
	Subtract = 21,
	Multiply = 22,
	Divide = 23,
	Reg1Bigger = 24,
	Reg1BiggerEqual = 25,
	Reg2Bigger = 26,
	Reg2BiggerEqual = 27,
	RegEqual = 28,
	RegDiff = 29,
	Jump = 30,
	CompareTrueFalse = 31,
	Wait = 32,
	coutReg1 = 40,
	coutReg2 = 41,
	KeyInputToCache = 42,
	CopyRamToVRam = 50,
	CopyVRamToRam = 51,
	DrawRect = 52,
	DrawChar = 53,
	ClearGpuCmdBuffer = 55,
	ClearGpuVRam = 56,
};

void CPU::execute(u16 register) {
	u16 opCode = registerOP1;

	switch (opCode) {
	case Halt: { //Halt
		halt = true;
		break; }

	case LoadReg1Cache: { //Load register1 from cache
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		register0 = cache.memory[memPos];
		programCounter += 2;
		programCounter++;
		break; }

	case LoadReg2Cache: { //Load register2 from cache
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		register1 = cache.memory[memPos];
		programCounter += 2;
		programCounter++;
		break; }

	case LoadReg1Ram: { //Load register1 from ram
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		register0 = ram.memory[memPos];
		programCounter += 2;
		programCounter++;
		break; }

	case LoadReg2Ram: { //Load register2 from ram
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		register1 = ram.memory[memPos];
		programCounter += 2;
		programCounter++;
		break; }

	case ClearReg1: { //clear register1
		register0 = 0;
		programCounter++;
		break; }

	case ClearReg2: { //Clear register2
		register1 = 0;
		programCounter++;
		break; }

	case WriteReg0ToCache: { //Write from register0 to cache
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		cache.memory[memPos] = register0;
		programCounter += 2;
		programCounter++;
		break; }

	case WriteReg1ToCache: { //Write from register1 to cache
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		cache.memory[memPos] = register1;
		programCounter += 2;
		programCounter++;
		break; }

	case WriteReg0ToRam: { //Write from register0 to ram
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		ram.memory[memPos] = register0;
		programCounter += 2;
		programCounter++;
		break; }

	case WriteReg1ToRam: { //Write from register1 to ram
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		ram.memory[memPos] = register1;
		programCounter += 2;
		programCounter++;
		break; }

	case WriteCacheToRam: { //Write from cache to ram
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg2p1 = cache.memory[programCounter + 3];
		byte arg2p2 = cache.memory[programCounter + 4];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		u16 memPos2 = arg2p1 + (arg2p2 * 256);
		ram.memory[memPos2] = cache.memory[memPos];
		programCounter++;
		break; }

	case WriteRamToCache: { //Write from ram to cache
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg2p1 = cache.memory[programCounter + 3];
		byte arg2p2 = cache.memory[programCounter + 4];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		u16 memPos2 = arg2p2 + (arg2p2 * 256);
		cache.memory[memPos2] = ram.memory[memPos];
		programCounter++;
		break; }

	case Sum: { //Sum register1 and register2
		register0 = register0 + register1;
		programCounter++;
		break; }

	case Subtract: { //Subtract register1 and register2
		register0 = register0 - register1;
		programCounter++;
		break; }

	case Multiply: { //Multiply register1 and register2
		register0 = register0 * register1;
		programCounter++;
		break; }

	case Divide: { //Divide register1 and register2
		register0 = register0 / register1;
		programCounter++;
		break; }

	case Reg1Bigger: { //Check if register1 is bigger than register2
		register0 = register0 > register1;
		programCounter++;
		break; }

	case Reg1BiggerEqual: { //Check if register1 is bigger or equal to register2
		register0 = register0 >= register1;
		programCounter++;
		break; }

	case Reg2Bigger: { //Check if register2 is bigger than register1
		register0 = register0 < register1;
		programCounter++;
		break; }

	case Reg2BiggerEqual: { //Check if register2 is bigger or equal to register1
		register0 = register0 <= register1;
		programCounter++;
		break; }

	case RegEqual: { //Check if register1 and register2 have same values
		register0 = register0 == register1;
		programCounter++;
		break; }

	case RegDiff: { //Check if register1 is different than register2
		register0 = register0 != register1;
		programCounter++;
		break; }

	case Jump: { //Jump to place in code
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		programCounter = memPos;
		break; }

	case CompareTrueFalse: { //Compare to true/false
		byte condition = cache.memory[programCounter + 1];
		byte arg1p1 = cache.memory[programCounter + 2];
		byte arg1p2 = cache.memory[programCounter + 3];
		u16 memPos = arg1p1 + (arg1p2 * 256) - 1;
		if (register0 == condition) {
			programCounter = memPos;
		}else if(register0 != condition){	
			programCounter += 3;
		}
		programCounter++;
		break; }

	case Wait: { //Wait a number of cycles
		u16 waitTime = cache.memory[programCounter + 1];
		if (currentTime < waitTime) {
			currentTime++;
		}else if (currentTime >= waitTime) {
			currentTime = 0;
			programCounter += 2;
			programCounter++;
		}
		break; }

	case coutReg1: { //Cout from register1
		std::cout << register0 << '\n';
		programCounter++;
		break; }

	case coutReg2: { //Cout from register2
		std::cout << register1 << '\n';
		programCounter++;
		break; }

	case KeyInputToCache: { //Write from keyboard input register to place in cache
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		cache.memory[memPos] = keyboardRegister;
		programCounter += 2;
		programCounter++;
		break; }

	case CopyRamToVRam: { //Copy from RAM to VRAM
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg2p1 = cache.memory[programCounter + 3];
		byte arg2p2 = cache.memory[programCounter + 4];
		byte arg3p1 = cache.memory[programCounter + 5];
		byte arg3p2 = cache.memory[programCounter + 6];

		gpu.commandBuffer.memory[gpu.commandCounter] = 1;

		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 0] = arg1p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 1] = arg1p2;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 2] = arg2p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 3] = arg2p2;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 4] = arg3p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 5] = arg3p2;

		programCounter += 6;
		programCounter++;
		break; }

	case CopyVRamToRam: { //Copy from VRAM to RAM
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg2p1 = cache.memory[programCounter + 3];
		byte arg2p2 = cache.memory[programCounter + 4];
		byte arg3p1 = cache.memory[programCounter + 5];
		byte arg3p2 = cache.memory[programCounter + 6];

		gpu.commandBuffer.memory[gpu.commandCounter] = 1;

		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 0] = arg1p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 1] = arg1p2;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 2] = arg2p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 3] = arg2p2;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 4] = arg3p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 5] = arg3p2;

		programCounter += 6;
		programCounter++;
		break; }

	case DrawRect: { //Draw rectangle
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg2p1 = cache.memory[programCounter + 3];
		byte arg2p2 = cache.memory[programCounter + 4];
		byte arg3p1 = cache.memory[programCounter + 5];
		byte arg3p2 = cache.memory[programCounter + 6];
		byte arg4p1 = cache.memory[programCounter + 7];
		byte arg4p2 = cache.memory[programCounter + 8];
		byte arg5 = cache.memory[programCounter +  9];
		byte arg6 = cache.memory[programCounter + 10];
		byte arg7 = cache.memory[programCounter + 11];

		gpu.commandBuffer.memory[gpu.commandCounter] = 3;

		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  0] = arg1p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  1] = arg1p2;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  2] = arg2p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  3] = arg2p2;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  4] = arg3p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  5] = arg3p2;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  6] = arg4p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  7] = arg4p2;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  8] = arg5;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  9] = arg6;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 10] = arg7;

		programCounter += 11;
		programCounter++;
		break; }

	case DrawChar: { //Draw a character
		byte characterPositionp1 = cache.memory[programCounter + 1];
		byte characterPositionp2 = cache.memory[programCounter + 2];
		u16 characterPosition = characterPositionp1 + (characterPositionp2 * 256);
		byte characterCode = cache.memory[characterPosition];
		byte xPos = cache.memory[programCounter + 3];
		byte yPos = cache.memory[programCounter + 4];
		byte r = cache.memory[programCounter + 5];
		byte g = cache.memory[programCounter + 6];
		byte b = cache.memory[programCounter + 7];

		gpu.commandBuffer.memory[gpu.commandCounter] = 4;

		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 0] = characterCode;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 1] = xPos;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 2] = yPos;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 3] = r;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 4] = g;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 5] = b;

		programCounter += 7;
		programCounter++;
		break; }

	case ClearGpuCmdBuffer: { //Clear GPU commandBuffer
		gpu.commandBuffer.memory[gpu.commandCounter] = 5;
		programCounter++;
		break; }

	case ClearGpuVRam: { //Clear GPU VRAM
		if (gpu.functionCounter == gpu.firstAvailableByte) {
			gpu.commandBuffer.memory[gpu.commandCounter] = 6;
			programCounter++;
		}
		break; }
	}
}

void CPU::tick() {
	if (halt == false) {
		registerOP1 = cache.memory[programCounter];
		execute(register1);
		
	}
}