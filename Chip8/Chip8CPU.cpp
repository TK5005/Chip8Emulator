#include "stdafx.h"
#include "Chip8CPU.h"

Chip8CPU::Chip8CPU(Chip8Memory* memory)
	:pMemory(memory)
{
	initialize();
}

Chip8CPU::~Chip8CPU()
{

}

void Chip8CPU::cycle()
{
	unsigned short opcode = pMemory->getValue(registers.PC) << 8 | pMemory->getValue(registers.PC + 1);

	executeOpcode(opcode);

	// Update Delay Timer
	if (registers.DT > 0)
	{
		--registers.DT;
	}

	// Update Sound Timer
	if (registers.ST > 0)
	{
		if (registers.ST == 1)
		{
			//TODO: Make something happen when sound timer hits 1.
		}
		--registers.ST;
	}
}

void Chip8CPU::reset()
{
	clearRegisters();
	clearStack();
	pMemory->clear();
	loadFontSet();

	// Seed random number generator
	srand(time(NULL));
}

unsigned short Chip8CPU::getRegisterI()
{
	return registers.I;
}

unsigned char Chip8CPU::getRegisterV(int regNum)
{
	return registers.V[regNum];
}

unsigned short Chip8CPU::getRegisterPC()
{
	return registers.PC;
}

char Chip8CPU::getRegisterSP()
{
	return registers.SP;
}

char Chip8CPU::getRegisterDT()
{
	return registers.DT;
}

char Chip8CPU::getRegisterST()
{
	return registers.ST;
}

std::stack<unsigned short> Chip8CPU::getStack()
{
	return stack;
}

void Chip8CPU::initialize()
{
	loadOpcodes();
	reset();
}

void Chip8CPU::clearRegisters()
{
	for (int i = 0; i < NUM_REGISTERS; i++)
	{
		registers.V[i] = 0;
	}
	registers.I = 0;
	registers.PC = 0x200;
	registers.SP = 0;
	registers.DT = 0;
	registers.ST = 0;
}

void Chip8CPU::clearStack()
{
	while (!stack.empty())
	{
		stack.pop();
	}
}

void Chip8CPU::loadOpcodes()
{
	
	opcodeMap.emplace(0x00E0, &Chip8CPU::_00E0);
	opcodeMap.emplace(0x1000, &Chip8CPU::_1NNN);
	opcodeMap.emplace(0x00EE, &Chip8CPU::_00EE);
	opcodeMap.emplace(0x2000, &Chip8CPU::_2NNN);
	opcodeMap.emplace(0x3000, &Chip8CPU::_3XNN);
	opcodeMap.emplace(0x4000, &Chip8CPU::_4XNN);
	opcodeMap.emplace(0x5000, &Chip8CPU::_5XY0);
	opcodeMap.emplace(0x6000, &Chip8CPU::_6XNN);
	opcodeMap.emplace(0x7000, &Chip8CPU::_7XNN);
	opcodeMap.emplace(0x8000, &Chip8CPU::_8XY0);
	opcodeMap.emplace(0x8001, &Chip8CPU::_8XY1);
	opcodeMap.emplace(0x8002, &Chip8CPU::_8XY2);
	opcodeMap.emplace(0x8003, &Chip8CPU::_8XY3);
	opcodeMap.emplace(0x8004, &Chip8CPU::_8XY4);
	opcodeMap.emplace(0x8005, &Chip8CPU::_8XY5);
	opcodeMap.emplace(0x8006, &Chip8CPU::_8XY6);
	opcodeMap.emplace(0x8007, &Chip8CPU::_8XY7);
	opcodeMap.emplace(0x800E, &Chip8CPU::_8XYE);
	opcodeMap.emplace(0x9000, &Chip8CPU::_9XY0);
	opcodeMap.emplace(0xA000, &Chip8CPU::_ANNN);
	opcodeMap.emplace(0xB000, &Chip8CPU::_BNNN);
	opcodeMap.emplace(0xC000, &Chip8CPU::_CXNN);
	opcodeMap.emplace(0xD000, &Chip8CPU::_DXYN);
	opcodeMap.emplace(0xE00E, &Chip8CPU::_EX9E);
	opcodeMap.emplace(0xE001, &Chip8CPU::_EXA1);
	opcodeMap.emplace(0xF007, &Chip8CPU::_FX07);
	opcodeMap.emplace(0xF00A, &Chip8CPU::_FX0A);
	opcodeMap.emplace(0xF015, &Chip8CPU::_FX15);
	opcodeMap.emplace(0xF018, &Chip8CPU::_FX18);
	opcodeMap.emplace(0xF01E, &Chip8CPU::_FX1E);
	opcodeMap.emplace(0xF029, &Chip8CPU::_FX29);
	opcodeMap.emplace(0xF033, &Chip8CPU::_FX33);
	opcodeMap.emplace(0xF055, &Chip8CPU::_FX55);
	opcodeMap.emplace(0xF065, &Chip8CPU::_FX65);
}

void Chip8CPU::loadFontSet()
{
	for (int i = 0; i < FONTSET_SIZE; i++)
	{
		pMemory->setValue(i, chip8_fontset[i]);
	}
}

void Chip8CPU::executeOpcode(unsigned short opcode)
{
	unsigned short searchVal = 0x0000;

	switch (opcode & 0xF000)
	{
	case 0x0000:
		searchVal = opcode;
		break;
	case 0x8000:
		searchVal = (opcode & 0xF00F);
		break;
	case 0xE000:
		searchVal = (opcode & 0xF00F);
		break;
	case 0xF000:
		searchVal = (opcode & 0xF0FF);
		break;
	default:
		searchVal = (opcode & 0xF000);
		break;
	}

	auto iter = opcodeMap.find(searchVal);
	if (iter == opcodeMap.end())
	{
		printf("Opcode %X not found.\n", opcode);
		return;
	}
	(this->*iter->second)(opcode);
}

//TODO: Write Method and Test
void Chip8CPU::_00E0(unsigned short opcode)
{

}

/*
// 0x00EE
// Returns from a subroutine
*/
void Chip8CPU::_00EE(unsigned short opcode)
{
	registers.PC = stack.top();
	stack.pop();
}

/*
// 0x1NNN
// Set Program Counter to address NNN
*/
void Chip8CPU::_1NNN(unsigned short opcode)
{
	registers.PC = (opcode & 0x0FFF);
}

/*
// 0x2NNN
// Puts the Program Counter on the stack
// Sets the Program Counter to NNN
*/
void Chip8CPU::_2NNN(unsigned short opcode)
{
	stack.push(registers.PC);
	registers.PC = (opcode & 0x0FFF);
}

/*
// 0x3XNN
// Skip next instruction if VX = NN
// The interpreter compares register VX to NN, and if they are equal, increments the program counter by 2.
*/
void Chip8CPU::_3XNN(unsigned short opcode)
{
	int vIndex = ((opcode & 0x0F00) >> 8);
	unsigned short address = (opcode & 0x00FF);
	registers.PC += (registers.V[vIndex] == address) ? 4 : 2;
}

/*
// 0x4XNN
// Skip next instruction if VX != NN.
// The interpreter compares register VX to NN, and if they are not equal, increments the program counter by 2.
*/
void Chip8CPU::_4XNN(unsigned short opcode)
{
	int vIndex = ((opcode & 0x0F00) >> 8);
	unsigned short address = (opcode & 0x00FF);
	registers.PC += (registers.V[vIndex] != address) ? 4 : 2;
}

//TODO: Write Method and Test
/*
// 0x5XY0
// Skip next instruction if VX = VY.
// The interpreter compares register VX to register VY, and if they are equal, increments the program counter by 2.
*/
void Chip8CPU::_5XY0(unsigned short opcode)
{
	int vIndex1 = ((opcode & 0x0F00) >> 8);
	int vIndex2 = ((opcode & 0x00F0) >> 4);
	registers.PC += (registers.V[vIndex1] == registers.V[vIndex2]) ? 4 : 2;
}

/*
// 0x6XNN
// Set VX = NN.
// The interpreter puts the value NN into register VX.
*/
void Chip8CPU::_6XNN(unsigned short opcode)
{
	int vIndex = ((opcode & 0x0F00) >> 8);
	unsigned short address = (opcode & 0x00FF);
	registers.V[vIndex] = address;
	registers.PC += 2;
}

/*
// 0x7XNN
// Set Vx = VX + NN.
// Adds the value NN to the value of register VX, then stores the result in VX. 
*/
void Chip8CPU::_7XNN(unsigned short opcode)
{
	int vIndex = ((opcode & 0x0F00) >> 8);
	unsigned short address = (opcode & 0x00FF);
	registers.V[vIndex] += address;
	registers.PC += 2;
}

/*
// 0x8XY0
// Set Vx = Vy.
// Stores the value of register Vy in register Vx.
*/
void Chip8CPU::_8XY0(unsigned short opcode)
{
	int vIndex1 = ((opcode & 0x0F00) >> 8);
	int vIndex2 = ((opcode & 0x00F0) >> 4);
	registers.V[vIndex1] = registers.V[vIndex2];
	registers.PC += 2;
}

/*
// 0x8XY1
// Set Vx = Vx OR Vy.
// Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx.
*/
void Chip8CPU::_8XY1(unsigned short opcode)
{
	int vIndex1 = ((opcode & 0x0F00) >> 8);
	int vIndex2 = ((opcode & 0x00F0) >> 4);
	registers.V[vIndex1] |= registers.V[vIndex2];
	registers.PC += 2;
}

/*
// 0x8XY2
// Set Vx = Vx AND Vy.
// Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx.
*/
void Chip8CPU::_8XY2(unsigned short opcode)
{
	int vIndex1 = ((opcode & 0x0F00) >> 8);
	int vIndex2 = ((opcode & 0x00F0) >> 4);
	registers.V[vIndex1] &= registers.V[vIndex2];
	registers.PC += 2;
}

/*
// 0x8XY3
// Set Vx = Vx XOR Vy.
// Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx.
*/
void Chip8CPU::_8XY3(unsigned short opcode)
{
	int vIndex1 = ((opcode & 0x0F00) >> 8);
	int vIndex2 = ((opcode & 0x00F0) >> 4);
	registers.V[vIndex1] ^= registers.V[vIndex2];
	registers.PC += 2;
}

/*
// 0x8XY4
// Set Vx = Vx + Vy, set VF = carry.
// The values of Vx and Vy are added together.
// If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0.
// Only the lowest 8 bits of the result are kept, and stored in Vx.
*/
void Chip8CPU::_8XY4(unsigned short opcode)
{
	int vIndex1 = ((opcode & 0x0F00) >> 8);
	int vIndex2 = ((opcode & 0x00F0) >> 4);
	unsigned short value = registers.V[vIndex1] + registers.V[vIndex2];
	registers.V[vIndex1] = (value & 0x00FF);
	registers.V[0xF] = (value > 0xFF) ? 1 : 0;
	registers.PC += 2;
}

/*
// 0x8XY5
// Set Vx = Vx - Vy, set VF = NOT borrow.
// If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
*/
void Chip8CPU::_8XY5(unsigned short opcode)
{
	int X = ((opcode & 0x0F00) >> 8);
	int Y = ((opcode & 0x00F0) >> 4);
	registers.V[0xF] = (registers.V[X] > registers.V[Y]) ? 1 : 0;
	registers.V[X] = registers.V[X] - registers.V[Y];
	registers.PC += 2;
}

/*
// 0x8XY6
// Set Vx = Vx SHR 1.
// If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
*/
void Chip8CPU::_8XY6(unsigned short opcode)
{
	int X = ((opcode & 0x0F00) >> 8);
	unsigned short val = registers.V[X];
	registers.V[0xF] = registers.V[X] & 0x1;
	registers.V[X] = registers.V[X] >> 1;
	registers.PC += 2;
}

//TODO: Write Method and Test
/*
// 0x8XY7
// Set Vx = Vy - Vx, set VF = NOT borrow.
// If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
*/
void Chip8CPU::_8XY7(unsigned short opcode)
{
	int X = ((opcode & 0x0F00) >> 8);
	int Y = ((opcode & 0x00F0) >> 4);
	registers.V[0xF] = (registers.V[Y] > registers.V[X]) ? 1 : 0;
	registers.V[X] = registers.V[Y] - registers.V[X];
	registers.PC += 2;
}

/*
// 0x8XYE
// Set Vx = Vx SHL 1.
// If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
*/
void Chip8CPU::_8XYE(unsigned short opcode)
{
	int X = ((opcode & 0x0F00) >> 8);
	registers.V[0xF] = registers.V[X] >> 7;
	registers.V[X] <<= 1;
	registers.PC += 2;
}

/*
// 0x9XY0
// Skip next instruction if Vx != Vy.
// The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
*/
void Chip8CPU::_9XY0(unsigned short opcode)
{
	int X = ((opcode & 0x0F00) >> 8);
	int Y = ((opcode & 0x00F0) >> 4);
	registers.PC += (registers.V[X] != registers.V[Y]) ? 4 : 2;
}

//TODO: Write Method and Test
/*
// 0xANNN
// Set I = nnn.
// The value of register I is set to nnn.
*/
void Chip8CPU::_ANNN(unsigned short opcode)
{
	registers.I = (opcode & 0x0FFF);
	registers.PC += 2;
}

/*
// 0xBNNN
// Jump to location nnn + V0.
// The program counter is set to nnn plus the value of V0.
*/
void Chip8CPU::_BNNN(unsigned short opcode)
{
	registers.PC = (opcode & 0x0FFF) + registers.V[0];
}

//TODO: Write Method and Test
/*
// 0xCXNN
// Set Vx = random byte AND kk.
// The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk.
// The results are stored in Vx. See instruction 8xy2 for more information on AND.
*/
void Chip8CPU::_CXNN(unsigned short opcode)
{

}

//TODO: Write Method and Test
/*
// 0xDXYN
// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
// The interpreter reads n bytes from memory, starting at the address stored in I.
// These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
// Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1,
// otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates of the
// display, it wraps around to the opposite side of the screen. See instruction 8xy3 for more information
// on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.
*/
void Chip8CPU::_DXYN(unsigned short opcode)
{

}

//TODO: Write Method and Test
/*
// 0xEX9E
// Skip next instruction if key with the value of Vx is pressed.
// Checks the keyboard, and if the key corresponding to the value of Vx is currently
// in the down position, PC is increased by 2.
*/
void Chip8CPU::_EX9E(unsigned short opcode)
{

}

//TODO: Write Method and Test
/*
// 0xEXA1
// Skip next instruction if key with the value of Vx is not pressed.
// Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position,
// PC is increased by 2.
*/
void Chip8CPU::_EXA1(unsigned short opcode)
{

}

/*
// 0xFX07
// Set Vx = delay timer value.
// The value of DT is placed into Vx.
*/
void Chip8CPU::_FX07(unsigned short opcode)
{
	int X = ((opcode & 0x0F00) >> 8);
	registers.V[X] = registers.DT;
	registers.PC += 2;
}

//TODO: Write Method and Test
/*
// 0xFX0A
// Wait for a key press, store the value of the key in Vx.
// All execution stops until a key is pressed, then the value of that key is stored in Vx.
*/
void Chip8CPU::_FX0A(unsigned short opcode)
{

}

/*
// 0xFX15
// Set delay timer = Vx.
// DT is set equal to the value of Vx.
*/
void Chip8CPU::_FX15(unsigned short opcode)
{
	int X = ((opcode & 0x0F00) >> 8);
	registers.DT = registers.V[X];
	registers.PC += 2;
}

//TODO: Write Method and Test
/*
// 0xFX18
// Set sound timer = Vx.
// ST is set equal to the value of Vx.
*/
void Chip8CPU::_FX18(unsigned short opcode)
{
}

//TODO: Write Method and Test
/*
// 0xFX1E
// Set I = I + Vx.
// The values of I and Vx are added, and the results are stored in I.
*/
void Chip8CPU::_FX1E(unsigned short opcode)
{
}

//TODO: Write Method and Test
/*
// 0xFX29
// Set I = location of sprite for digit Vx.
// The value of I is set to the location for the hexadecimal
// sprite corresponding to the value of Vx. See section 2.4, Display,
// for more information on the Chip-8 hexadecimal font.
*/
void Chip8CPU::_FX29(unsigned short opcode)
{
}

//TODO: Write Method and Test
/*
// 0xFX33
// Store BCD representation of Vx in memory locations I, I+1, and I+2.
// The interpreter takes the decimal value of Vx, and places the
// hundreds digit in memory at location in I, the tens digit at
// location I+1, and the ones digit at location I+2.
*/
void Chip8CPU::_FX33(unsigned short opcode)
{
}

//TODO: Write Method and Test
/*
// 0xFX55
// Store registers V0 through Vx in memory starting at location I.
// The interpreter copies the values of registers V0 through Vx into
// memory, starting at the address in I.
*/
void Chip8CPU::_FX55(unsigned short opcode)
{
}

//TODO: Write Method and Test
/*
// 0xFX65
// Read registers V0 through Vx from memory starting at location I.
// The interpreter reads values from memory starting at location I
// into registers V0 through Vx.
*/
void Chip8CPU::_FX65(unsigned short opcode)
{
}
