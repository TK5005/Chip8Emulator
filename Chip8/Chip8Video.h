#pragma once
#include <SFML/Graphics.hpp>
#include "Chip8GraphicsMemory.h"

class Chip8Video : public sf::RenderWindow
{
public:
	Chip8Video(Chip8GraphicsMemory*, int pixelSize, unsigned int width, unsigned int height, sf::String title);
	void drawImage();
private:
	int pPixelSize = 8;
	Chip8GraphicsMemory* pGfxMemory;
};