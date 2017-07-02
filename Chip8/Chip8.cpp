// Chip8.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Chip8CPU.h"
#include "Chip8Memory.h"
#include "Chip8Input.h"
#include "Chip8Video.h"
#include <stdio.h>
#include <iostream>
#include <SFML\Graphics.hpp>

int main()
{
	const int ROWS = 1024;
	const int COLUMNS = 2048;
	const int PIXEL_SIZE = 32;
	sf::String title = "Chip8";

	// Initialize classes
	Chip8Memory memory;
	Chip8Input input;
	Chip8GraphicsMemory gfxMemory;
	Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
	Chip8Video video(&gfxMemory, PIXEL_SIZE, COLUMNS, ROWS, title);

	// Load program
	bool fileLoaded = memory.loadOpcodes(0x200, "invaders.c8");

	if (fileLoaded)
	{
		sf::Clock cpuClock;
		sf::Clock timerClock;

		while (video.isOpen())
		{
			if (cpuClock.getElapsedTime().asMicroseconds() > 1852)
			{
				// emulate cycle
				chip8CPU.cycle();
				// set key presses
				input.setKeyPressed(0x00, sf::Keyboard::isKeyPressed(sf::Keyboard::Num1));
				input.setKeyPressed(0x01, sf::Keyboard::isKeyPressed(sf::Keyboard::Num2));
				input.setKeyPressed(0x02, sf::Keyboard::isKeyPressed(sf::Keyboard::Num3));
				input.setKeyPressed(0x03, sf::Keyboard::isKeyPressed(sf::Keyboard::Num4));
				input.setKeyPressed(0x04, sf::Keyboard::isKeyPressed(sf::Keyboard::Q));
				input.setKeyPressed(0x05, sf::Keyboard::isKeyPressed(sf::Keyboard::W));
				input.setKeyPressed(0x06, sf::Keyboard::isKeyPressed(sf::Keyboard::E));
				input.setKeyPressed(0x07, sf::Keyboard::isKeyPressed(sf::Keyboard::R));
				input.setKeyPressed(0x08, sf::Keyboard::isKeyPressed(sf::Keyboard::A));
				input.setKeyPressed(0x09, sf::Keyboard::isKeyPressed(sf::Keyboard::S));
				input.setKeyPressed(0x0A, sf::Keyboard::isKeyPressed(sf::Keyboard::D));
				input.setKeyPressed(0x0B, sf::Keyboard::isKeyPressed(sf::Keyboard::F));
				input.setKeyPressed(0x0C, sf::Keyboard::isKeyPressed(sf::Keyboard::Z));
				input.setKeyPressed(0x0D, sf::Keyboard::isKeyPressed(sf::Keyboard::X));
				input.setKeyPressed(0x0E, sf::Keyboard::isKeyPressed(sf::Keyboard::C));

				if (chip8CPU.getDrawFlag())
				{
					video.drawImage();
					chip8CPU.clearDrawFlag();
				}
				cpuClock.restart();
			}

			if (timerClock.getElapsedTime().asMicroseconds() > 16667)
			{
				chip8CPU.updateTimers();
				timerClock.restart();
			}

			// set key presses
			input.setKeyPressed(0x00, sf::Keyboard::isKeyPressed(sf::Keyboard::Num1));
			input.setKeyPressed(0x01, sf::Keyboard::isKeyPressed(sf::Keyboard::Num2));
			input.setKeyPressed(0x02, sf::Keyboard::isKeyPressed(sf::Keyboard::Num3));
			input.setKeyPressed(0x03, sf::Keyboard::isKeyPressed(sf::Keyboard::Num4));
			input.setKeyPressed(0x04, sf::Keyboard::isKeyPressed(sf::Keyboard::Q));
			input.setKeyPressed(0x05, sf::Keyboard::isKeyPressed(sf::Keyboard::W));
			input.setKeyPressed(0x06, sf::Keyboard::isKeyPressed(sf::Keyboard::E));
			input.setKeyPressed(0x07, sf::Keyboard::isKeyPressed(sf::Keyboard::R));
			input.setKeyPressed(0x08, sf::Keyboard::isKeyPressed(sf::Keyboard::A));
			input.setKeyPressed(0x09, sf::Keyboard::isKeyPressed(sf::Keyboard::S));
			input.setKeyPressed(0x0A, sf::Keyboard::isKeyPressed(sf::Keyboard::D));
			input.setKeyPressed(0x0B, sf::Keyboard::isKeyPressed(sf::Keyboard::F));
			input.setKeyPressed(0x0C, sf::Keyboard::isKeyPressed(sf::Keyboard::Z));
			input.setKeyPressed(0x0D, sf::Keyboard::isKeyPressed(sf::Keyboard::X));
			input.setKeyPressed(0x0E, sf::Keyboard::isKeyPressed(sf::Keyboard::C));

			//Check for window close
			sf::Event ev;
			while (video.pollEvent(ev))
			{
				if (ev.type == sf::Event::Closed)
				{
					video.close();
				}
			}
		}
	}
	else {
		std::cin.get();
	}
    return 0;
}