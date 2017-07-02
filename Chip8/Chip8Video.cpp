#include "stdafx.h"
#include "Chip8Video.h"

Chip8Video::Chip8Video(Chip8GraphicsMemory* gfxMemory, int pixelSize, unsigned int width, unsigned int height, sf::String title) :RenderWindow(sf::VideoMode(width, height), title),
pGfxMemory(gfxMemory),
pPixelSize(pixelSize)
{
	clear();
	setupPixels();
	display();
}

void Chip8Video::setupPixels()
{
	for (int y = 0; y < 32; ++y)
	{
		for (int x = 0; x < 64; ++x)
		{
			sf::RectangleShape pixel(sf::Vector2f(pPixelSize, pPixelSize));
			pixel.setPosition(x*pPixelSize, y*pPixelSize);
			pixel.setFillColor(sf::Color::Black);
			pixels[(y * 64) + x] = pixel;
			draw(pixel);
		}
	}
}

void Chip8Video::drawImage()
{
	for (int y = 0; y < 32; ++y)
	{
		for (int x = 0; x < 64; ++x)
		{

			if (pGfxMemory->getValue((y * 64) + x) == 0)
				pixels[(y * 64) + x].setFillColor(sf::Color::Black);
			else
				pixels[(y * 64) + x].setFillColor(sf::Color::White);
			draw(pixels[(y * 64) + x]);
		}
	}

	display();
}