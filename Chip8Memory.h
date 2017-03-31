#ifndef CHIP8MEMORY_H
#define CHIP8MEMORY_H

#include <exception>

namespace Chip8MemoryException {
	class out_of_range : public std::exception
	{
		virtual const char* what() const throw()
		{
			return "Chip8MemoryException: Out of Range";
		}
	};
}

/**
*
* Class: Chip8Memory
* Description: Representation of the memory in a Chip8 computing system.
*
*/
class Chip8Memory
{
public:
	static const unsigned short MEMORY_SIZE = 4096;
	Chip8Memory();
	~Chip8Memory();
	unsigned char getValue(unsigned short location);
	void setValue(unsigned short location, unsigned char value);
	void clear();
	bool isClear();
private:
	unsigned char memory[MEMORY_SIZE];
};

#endif