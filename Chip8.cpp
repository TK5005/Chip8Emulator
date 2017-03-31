/**
*
* Program entry
* 
*/

#include "Chip8CPU.h"
#include "Chip8Memory.h"
#include <stdio.h>
#include <iostream>

int main()
{
	Chip8Memory memory;
	Chip8CPU chip8CPU(&memory);
	memory.setValue(0x200, 0x32);
	memory.setValue(0x201, 0x53);
	chip8CPU.cycle();
	//TODO: Draw Graphics
	//TODO: Update Key Presses
	return 0;
}

