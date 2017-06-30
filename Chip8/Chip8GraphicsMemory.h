#pragma once

class Chip8GraphicsMemory
{
public:
	static const int MEMORY_SIZE = 64 * 32;
	Chip8GraphicsMemory();
	~Chip8GraphicsMemory();
	void setValue(unsigned short position, unsigned char value);
	char getValue(unsigned short position);
	void clear();
private:
	unsigned char gfx[MEMORY_SIZE];
};