#ifndef CHIP8CPU_H
#define CHIP8CPU_H

#include "Chip8Memory.h"
#include <stack>
#include <time.h>
#include <unordered_map>

class Chip8CPU
{
public:
	static const unsigned short MEMORY_SIZE = 4096;
	static const unsigned short NUM_REGISTERS = 16;
	static const unsigned short STACK_SIZE = 16;
	static const unsigned short GRAPHICS_PIXELS = (64 * 32);
	static const unsigned short NUM_KEYS = 16;
	static const unsigned short FONTSET_SIZE = 80;
	Chip8CPU(Chip8Memory* memory);
	~Chip8CPU();
	void cycle();
	void reset();
	typedef void (Chip8CPU::*ScriptFunction)(unsigned short);
	typedef std::unordered_map<unsigned short, ScriptFunction> OpcodeMap;
	unsigned short getRegisterI();
	unsigned char getRegisterV(int regNum);
	unsigned short getRegisterPC();
	char getRegisterSP();
	char getRegisterDT();
	char getRegisterST();
	std::stack<unsigned short> getStack();
private:
	void initialize();
	void clearRegisters();
	void clearStack();
	void loadOpcodes();
	void loadFontSet();
	void executeOpcode(unsigned short opcode);
	struct
	{
		unsigned char V[NUM_REGISTERS];
		unsigned short I;
		unsigned short PC;
		unsigned char SP;
		unsigned char DT;
		unsigned char ST;
	} registers;

	Chip8Memory* pMemory;

	std::stack<unsigned short> stack;	// Stack
	OpcodeMap opcodeMap;
	unsigned char chip8_fontset[FONTSET_SIZE] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};
	void _00E0(unsigned short);
	void _00EE(unsigned short);
	void _1NNN(unsigned short);
	void _2NNN(unsigned short);
	void _3XNN(unsigned short);
	void _4XNN(unsigned short);
	void _5XY0(unsigned short);
	void _6XNN(unsigned short);
	void _7XNN(unsigned short);
	void _8XY0(unsigned short);
	void _8XY1(unsigned short);
	void _8XY2(unsigned short);
	void _8XY3(unsigned short);
	void _8XY4(unsigned short);
	void _8XY5(unsigned short);
	void _8XY6(unsigned short);
	void _8XY7(unsigned short);
	void _8XYE(unsigned short);
	void _9XY0(unsigned short);
	void _ANNN(unsigned short);
	void _BNNN(unsigned short);
	void _CXNN(unsigned short);
	void _DXYN(unsigned short);
	void _EX9E(unsigned short);
	void _EXA1(unsigned short);
	void _FX07(unsigned short);
	void _FX0A(unsigned short);
	void _FX15(unsigned short);
	void _FX18(unsigned short);
	void _FX1E(unsigned short);
	void _FX29(unsigned short);
	void _FX33(unsigned short);
	void _FX55(unsigned short);
	void _FX65(unsigned short);
};

#endif