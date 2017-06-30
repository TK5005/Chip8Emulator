#include "stdafx.h"
#include "Chip8Input.h"

Chip8Input::Chip8Input()
{
	clearKeys();
}

Chip8Input::~Chip8Input()
{

}

void Chip8Input::setKeyPressed(unsigned short key, bool value)
{
	if (key <= 0xF)
	{
		m_keys[key] = value;
	}
}

bool Chip8Input::getKeyPressed(unsigned short key)
{
	if (key <= 0xF)
	{
		return m_keys[key];
	}
	else {
		return false;
	}
}

void Chip8Input::clearKeys()
{
	for (int i = 0; i < 16; i++)
	{
		m_keys[i] = false;
	}
}
