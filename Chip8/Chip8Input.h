#pragma once
class Chip8Input
{
public:
	Chip8Input();
	~Chip8Input();
	
	void setKeyPressed(unsigned short, bool);
	bool getKeyPressed(unsigned short);
	void clearKeys();
private:
	bool m_keys[16];
};