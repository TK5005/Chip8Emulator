#include "stdafx.h"
#include "Chip8GraphicsMemory.h"

Chip8GraphicsMemory::Chip8GraphicsMemory()
{
	clear();
}

Chip8GraphicsMemory::~Chip8GraphicsMemory()
{
}

void Chip8GraphicsMemory::setValue(unsigned short position, unsigned char value)
{
	gfx[position] = value;
}

char Chip8GraphicsMemory::getValue(unsigned short position)
{
	return gfx[position];
}

void Chip8GraphicsMemory::clear()
{
	for (int i = 0; i < MEMORY_SIZE; ++i)
	{
		setValue(i, 0);
	}
}


