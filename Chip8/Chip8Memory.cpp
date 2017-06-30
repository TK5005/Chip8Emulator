#include "stdafx.h"
#include "Chip8Memory.h"

Chip8Memory::Chip8Memory()
{
	clear();
}

Chip8Memory::~Chip8Memory()
{
}

bool Chip8Memory::loadOpcodes(unsigned short startLocation, const char* filename)
{
	printf("Loading: %s\n", filename);

	// Open file
	FILE * pFile = fopen(filename, "rb");
	if (pFile == NULL)
	{
		fputs("File error", stderr);
		return false;
	}

	// Check file size
	fseek(pFile, 0, SEEK_END);
	long lSize = ftell(pFile);
	rewind(pFile);
	printf("Filesize: %d\n", (int)lSize);

	// Allocate memory to contain the whole file
	char * buffer = (char*)malloc(sizeof(char) * lSize);
	if (buffer == NULL)
	{
		fputs("Memory error", stderr);
		return false;
	}

	// Copy the file into the buffer
	size_t result = fread(buffer, 1, lSize, pFile);
	if (result != lSize)
	{
		fputs("Reading error", stderr);
		return false;
	}

	// Copy buffer to Chip8 memory
	if ((4096 - startLocation) > lSize)
	{
		for (int i = 0; i < lSize; ++i)
			memory[i + startLocation] = buffer[i];
		printf("File Loaded");
	}
	else
		printf("Error: ROM too big for memory");

	// Close file, free buffer
	fclose(pFile);
	free(buffer);

	return true;
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
