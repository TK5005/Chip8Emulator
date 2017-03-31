#include "stdafx.h"
#include "Chip8Memory.h"

Chip8Memory::Chip8Memory()
{
}

Chip8Memory::~Chip8Memory()
{
}

unsigned char Chip8Memory::getValue(unsigned short location)
{
	if (location >= 0 && location < MEMORY_SIZE)
	{
		return memory[location];
	}
	else {
		throw Chip8MemoryException::out_of_range();
	}
}

void Chip8Memory::setValue(unsigned short location, unsigned char value)
{
	if (location >= 0 && location < MEMORY_SIZE)
	{
		memory[location] = value;
	}
	else {
		throw Chip8MemoryException::out_of_range();
	}
}

void Chip8Memory::clear()
{
	for (int i = 0; i < MEMORY_SIZE; ++i)
	{
		setValue(i, 0);
	}
}

bool Chip8Memory::isClear()
{
	for (int i = 0; i < MEMORY_SIZE; i++)
	{
		if (memory[i] != 0)
		{
			return false;
		}
	}
	return true;
}
