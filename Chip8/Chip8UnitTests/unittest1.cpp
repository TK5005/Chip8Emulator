#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Chip8Memory.h"
#include "../Chip8CPU.h"
#include "../Chip8GraphicsMemory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Chip8UnitTests
{
	TEST_CLASS(Chip8CPUTests)
	{
	public:
		TEST_METHOD(InitializeAndResetTest)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			Assert::AreEqual((char)0, chip8CPU.getRegisterDT(), L"Chip8CPU Register DT should be 0", LINE_INFO());
			Assert::AreEqual((char)0, chip8CPU.getRegisterST(), L"Chip8CPU Register ST should be 0", LINE_INFO());
			Assert::AreEqual((char)0, chip8CPU.getRegisterSP(), L"Chip8CPU Register SP should be 0", LINE_INFO());
			Assert::AreEqual((int)0x200, (int)chip8CPU.getRegisterPC(), L"Chip8CPU Register PC should be 0x200", LINE_INFO());
			Assert::AreEqual((int)0, (int)chip8CPU.getRegisterI(), L"Chip8CPU Register I should be 0", LINE_INFO());
			for (int i = 0; i < chip8CPU.NUM_REGISTERS; i++)
			{
				Assert::AreEqual((unsigned char)0, chip8CPU.getRegisterV(i), L"Chip8CPU V Registers should be 0", LINE_INFO());
			}
			Assert::IsTrue(chip8CPU.getStack().empty(), L"Stack should be empty on initialization", LINE_INFO());
			
			unsigned char chip8_fontset[Chip8CPU::FONTSET_SIZE] =
			{
				0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
				0x20, 0x60, 0x20, 0x20, 0x70, // 1
				0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
				0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
				0x90, 0x90, 0xF0, 0x10, 0x10, // 4
				0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
				0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
				0xF0, 0x10, 0x20, 0x40, 0x40, // 7
				0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
				0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
				0xF0, 0x90, 0xF0, 0x90, 0x90, // A
				0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
				0xF0, 0x80, 0x80, 0x80, 0xF0, // C
				0xE0, 0x90, 0x90, 0x90, 0xE0, // D
				0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
				0xF0, 0x80, 0xF0, 0x80, 0x80  // F
			};

			for (int i = 0; i < Chip8CPU::FONTSET_SIZE; i++)
			{
				Assert::AreEqual(chip8_fontset[i], memory.getValue(i), L"Values in memory do not match the proper values in the font set.", LINE_INFO());
			}
		}

		TEST_METHOD(_00E0_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			unsigned short memLocation = 0x020;
			unsigned char val = 5;
			gfxMemory.setValue(memLocation, val);
			memory.setValue(0x200, 0x00);
			memory.setValue(0x201, 0xE0);
			chip8CPU.cycle();
			val = gfxMemory.getValue(memLocation);
			unsigned char targetVal = 0;
			Assert::AreEqual(targetVal, val, L"Memory not cleared.", LINE_INFO());
		}

		TEST_METHOD(_00EE_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			memory.setValue(0x200, 0x25);
			memory.setValue(0x201, 0x43);
			chip8CPU.cycle();
			Assert::AreEqual((int)0x200, (int)chip8CPU.getStack().top(), L"Stack should have a value of 0x200", LINE_INFO());
			Assert::AreEqual((int)0x543, (int)chip8CPU.getRegisterPC(), L"Program should have a value of 0x543", LINE_INFO());
			memory.setValue(0x543, 0x23);
			memory.setValue(0x544, 0x54);
			chip8CPU.cycle();
			Assert::AreEqual((int)0x543, (int)chip8CPU.getStack().top(), L"Stack should have a value of 0x543", LINE_INFO());
			Assert::AreEqual((int)0x354, (int)chip8CPU.getRegisterPC(), L"Program should have a value of 0x354", LINE_INFO());
			memory.setValue(0x354, 0x00);
			memory.setValue(0x355, 0xEE);
			chip8CPU.cycle();
			Assert::AreEqual((int)0x200, (int)chip8CPU.getStack().top(), L"Stack should have a value of 0x200", LINE_INFO());
			Assert::AreEqual((int)0x543, (int)chip8CPU.getRegisterPC(), L"Program should have a value of 0x543", LINE_INFO());
			memory.setValue(0x543, 0x00);
			memory.setValue(0x544, 0xEE);
			chip8CPU.cycle();
			Assert::IsTrue(chip8CPU.getStack().empty(), L"Stack should be empty", LINE_INFO());
			Assert::AreEqual((int)0x200, (int)chip8CPU.getRegisterPC(), L"Program should have a value of 0x200", LINE_INFO());
		}

		TEST_METHOD(_1NNN_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			memory.setValue(0x200, 0x18);
			memory.setValue(0x201, 0x53);
			chip8CPU.cycle();
			Assert::AreEqual((int)0x853, (int)chip8CPU.getRegisterPC(), L"Program Counter does not equal 0x853", LINE_INFO());
			chip8CPU.reset();
			memory.setValue(0x200, 0x1F);
			memory.setValue(0x201, 0x35);
			chip8CPU.cycle();
			Assert::AreEqual((int)0xF35, (int)chip8CPU.getRegisterPC(), L"Program Counter does not equal 0xF35", LINE_INFO());
		}

		TEST_METHOD(_2NNN_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			memory.setValue(0x200, 0x25);
			memory.setValue(0x201, 0x43);
			chip8CPU.cycle();
			Assert::AreEqual((int)0x200, (int)chip8CPU.getStack().top(), L"Stack should have a value of 0x200", LINE_INFO());
			Assert::AreEqual((int)0x543, (int)chip8CPU.getRegisterPC(), L"Program should have a value of 0x543", LINE_INFO());
		}

		TEST_METHOD(_3XNN_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x35);
			memory.setValue(0x202, 0x30);
			memory.setValue(0x203, 0x22);
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x204, (int)chip8CPU.getRegisterPC(), L"PC is incorrect", LINE_INFO());
			chip8CPU.reset();
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x35);
			memory.setValue(0x202, 0x30);
			memory.setValue(0x203, 0x35);
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x206, (int)chip8CPU.getRegisterPC(), L"PC is incorrect", LINE_INFO());
		}

		TEST_METHOD(_4XNN_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x35);
			memory.setValue(0x202, 0x40);
			memory.setValue(0x203, 0x22);
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x206, (int)chip8CPU.getRegisterPC(), L"PC is incorrect", LINE_INFO());
			chip8CPU.reset();
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x35);
			memory.setValue(0x202, 0x40);
			memory.setValue(0x203, 0x35);
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x204, (int)chip8CPU.getRegisterPC(), L"PC is incorrect", LINE_INFO());
		}

		TEST_METHOD(_5XY0_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x35);
			memory.setValue(0x202, 0x61);
			memory.setValue(0x203, 0x35);
			memory.setValue(0x204, 0x50);
			memory.setValue(0x205, 0x10);
			chip8CPU.cycle();
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x208, (int)chip8CPU.getRegisterPC(), L"PC is incorrect", LINE_INFO());
			chip8CPU.reset();
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x35);
			memory.setValue(0x202, 0x61);
			memory.setValue(0x203, 0x22);
			memory.setValue(0x204, 0x50);
			memory.setValue(0x205, 0x10);
			chip8CPU.cycle();
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x206, (int)chip8CPU.getRegisterPC(), L"PC is incorrect", LINE_INFO());
		}

		TEST_METHOD(_6XNN_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x35);
			chip8CPU.cycle();
			memory.setValue(0x202, 0x61);
			memory.setValue(0x203, 0x45);
			chip8CPU.cycle();
			memory.setValue(0x204, 0x62);
			memory.setValue(0x205, 0x23);
			chip8CPU.cycle();
			memory.setValue(0x206, 0x63);
			memory.setValue(0x207, 0x89);
			chip8CPU.cycle();
			memory.setValue(0x208, 0x64);
			memory.setValue(0x209, 0xFF);
			chip8CPU.cycle();
			memory.setValue(0x20A, 0x65);
			memory.setValue(0x20B, 0x73);
			chip8CPU.cycle();
			memory.setValue(0x20C, 0x66);
			memory.setValue(0x20D, 0x29);
			chip8CPU.cycle();
			memory.setValue(0x20E, 0x67);
			memory.setValue(0x20F, 0x29);
			chip8CPU.cycle();
			memory.setValue(0x210, 0x68);
			memory.setValue(0x211, 0x29);
			chip8CPU.cycle();
			memory.setValue(0x212, 0x69);
			memory.setValue(0x213, 0x29);
			chip8CPU.cycle();
			memory.setValue(0x214, 0x6A);
			memory.setValue(0x215, 0x29);
			chip8CPU.cycle();
			memory.setValue(0x216, 0x6B);
			memory.setValue(0x217, 0x29);
			chip8CPU.cycle();
			memory.setValue(0x218, 0x6C);
			memory.setValue(0x219, 0x29);
			chip8CPU.cycle();
			memory.setValue(0x21A, 0x6D);
			memory.setValue(0x21B, 0x29);
			chip8CPU.cycle();
			memory.setValue(0x21C, 0x6E);
			memory.setValue(0x21D, 0x29);
			chip8CPU.cycle();
			memory.setValue(0x21E, 0x6F);
			memory.setValue(0x21F, 0x29);
			chip8CPU.cycle();
			Assert::AreEqual((int)0x35, (int)chip8CPU.getRegisterV(0), L"Register 0 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x45, (int)chip8CPU.getRegisterV(1), L"Register 1 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x23, (int)chip8CPU.getRegisterV(2), L"Register 2 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x89, (int)chip8CPU.getRegisterV(3), L"Register 3 incorrect", LINE_INFO());
			Assert::AreEqual((int)0xFF, (int)chip8CPU.getRegisterV(4), L"Register 4 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x73, (int)chip8CPU.getRegisterV(5), L"Register 5 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x29, (int)chip8CPU.getRegisterV(6), L"Register 6 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x29, (int)chip8CPU.getRegisterV(7), L"Register 7 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x29, (int)chip8CPU.getRegisterV(8), L"Register 8 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x29, (int)chip8CPU.getRegisterV(9), L"Register 9 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x29, (int)chip8CPU.getRegisterV(10), L"Register 10 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x29, (int)chip8CPU.getRegisterV(11), L"Register 11 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x29, (int)chip8CPU.getRegisterV(12), L"Register 12 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x29, (int)chip8CPU.getRegisterV(13), L"Register 13 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x29, (int)chip8CPU.getRegisterV(14), L"Register 14 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x29, (int)chip8CPU.getRegisterV(15), L"Register 15 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x220, (int)chip8CPU.getRegisterPC(), L"PC Incorrect", LINE_INFO());
		}

		TEST_METHOD(_7XNN_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x35);
			memory.setValue(0x202, 0x70);
			memory.setValue(0x203, 0x22);
			memory.setValue(0x204, 0x61);
			memory.setValue(0x205, 0x19);
			memory.setValue(0x206, 0x71);
			memory.setValue(0x207, 0x02);
			chip8CPU.cycle();
			chip8CPU.cycle();
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x57, (int)chip8CPU.getRegisterV(0), L"Register 0 not correct", LINE_INFO());
			Assert::AreEqual((int)0x1B, (int)chip8CPU.getRegisterV(1), L"Register 1 not correct", LINE_INFO());
			Assert::AreEqual((int)0x208, (int)chip8CPU.getRegisterPC(), L"PC Incorrect", LINE_INFO());
		}

		TEST_METHOD(_8XY0_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x35);
			memory.setValue(0x202, 0x61);
			memory.setValue(0x203, 0x22);
			memory.setValue(0x204, 0x80);
			memory.setValue(0x205, 0x10);
			chip8CPU.cycle();
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x22, (int)chip8CPU.getRegisterV(0), L"Register 0 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x22, (int)chip8CPU.getRegisterV(1), L"Register 1 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x206, (int)chip8CPU.getRegisterPC(), L"PC Incorrect", LINE_INFO());
		}

		TEST_METHOD(_8XY1_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x35);
			memory.setValue(0x202, 0x61);
			memory.setValue(0x203, 0x22);
			memory.setValue(0x204, 0x80);
			memory.setValue(0x205, 0x11);
			chip8CPU.cycle();
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x37, (int)chip8CPU.getRegisterV(0), L"Register 0 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x22, (int)chip8CPU.getRegisterV(1), L"Register 1 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x206, (int)chip8CPU.getRegisterPC(), L"PC Incorrect", LINE_INFO());
		}

		TEST_METHOD(_8XY2_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x35);
			memory.setValue(0x202, 0x61);
			memory.setValue(0x203, 0x22);
			memory.setValue(0x204, 0x80);
			memory.setValue(0x205, 0x12);
			chip8CPU.cycle();
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x20, (int)chip8CPU.getRegisterV(0), L"Register 0 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x22, (int)chip8CPU.getRegisterV(1), L"Register 1 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x206, (int)chip8CPU.getRegisterPC(), L"PC Incorrect", LINE_INFO());
		}

		TEST_METHOD(_8XY3_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x35);
			memory.setValue(0x202, 0x61);
			memory.setValue(0x203, 0x22);
			memory.setValue(0x204, 0x80);
			memory.setValue(0x205, 0x13);
			chip8CPU.cycle();
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x17, (int)chip8CPU.getRegisterV(0), L"Register 0 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x22, (int)chip8CPU.getRegisterV(1), L"Register 1 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x206, (int)chip8CPU.getRegisterPC(), L"PC Incorrect", LINE_INFO());
		}

		TEST_METHOD(_8XY4_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0xFE);
			memory.setValue(0x202, 0x61);
			memory.setValue(0x203, 0x02);
			memory.setValue(0x204, 0x80);
			memory.setValue(0x205, 0x14);
			chip8CPU.cycle();
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x00, (int)chip8CPU.getRegisterV(0), L"Register 0 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x02, (int)chip8CPU.getRegisterV(1), L"Register 1 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x01, (int)chip8CPU.getRegisterV(0xF), L"Register VF incorrect", LINE_INFO());
			Assert::AreEqual((int)0x206, (int)chip8CPU.getRegisterPC(), L"PC Incorrect", LINE_INFO());
			chip8CPU.reset();
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x03);
			memory.setValue(0x202, 0x61);
			memory.setValue(0x203, 0x02);
			memory.setValue(0x204, 0x80);
			memory.setValue(0x205, 0x14);
			chip8CPU.cycle();
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x05, (int)chip8CPU.getRegisterV(0), L"Register 0 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x02, (int)chip8CPU.getRegisterV(1), L"Register 1 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x00, (int)chip8CPU.getRegisterV(0xF), L"Register VF incorrect", LINE_INFO());
			Assert::AreEqual((int)0x206, (int)chip8CPU.getRegisterPC(), L"PC Incorrect", LINE_INFO());
			chip8CPU.reset();
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0xFE);
			memory.setValue(0x202, 0x61);
			memory.setValue(0x203, 0x32);
			memory.setValue(0x204, 0x80);
			memory.setValue(0x205, 0x14);
			chip8CPU.cycle();
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x30, (int)chip8CPU.getRegisterV(0), L"Register 0 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x32, (int)chip8CPU.getRegisterV(1), L"Register 1 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x01, (int)chip8CPU.getRegisterV(0xF), L"Register VF incorrect", LINE_INFO());
			Assert::AreEqual((int)0x206, (int)chip8CPU.getRegisterPC(), L"PC Incorrect", LINE_INFO());
		}

		TEST_METHOD(_8XY5_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0xFE);
			memory.setValue(0x202, 0x61);
			memory.setValue(0x203, 0x02);
			memory.setValue(0x204, 0x80);
			memory.setValue(0x205, 0x15);
			chip8CPU.cycle();
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0xFC, (int)chip8CPU.getRegisterV(0), L"Register 0 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x02, (int)chip8CPU.getRegisterV(1), L"Register 1 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x01, (int)chip8CPU.getRegisterV(0xF), L"Register VF incorrect", LINE_INFO());
			Assert::AreEqual((int)0x206, (int)chip8CPU.getRegisterPC(), L"PC Incorrect", LINE_INFO());
			chip8CPU.reset();
			// Add 0x05 to Register V0
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x05);
			// Add 0x06 to Register V1
			memory.setValue(0x202, 0x61);
			memory.setValue(0x203, 0x06);
			// Run 0x8015
			memory.setValue(0x204, 0x80);
			memory.setValue(0x205, 0x15);
			chip8CPU.cycle();
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0xFF, (int)chip8CPU.getRegisterV(0), L"Register 0 incorrect when there is a borrow", LINE_INFO());
			Assert::AreEqual((int)0x06, (int)chip8CPU.getRegisterV(1), L"Register 1 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x00, (int)chip8CPU.getRegisterV(0xF), L"Register VF incorrect", LINE_INFO());
			Assert::AreEqual((int)0x206, (int)chip8CPU.getRegisterPC(), L"PC Incorrect", LINE_INFO());
		}

		TEST_METHOD(_8XY6_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			// Set V0 to 0xFE
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0xFE);
			// Call 0x8016
			memory.setValue(0x202, 0x80);
			memory.setValue(0x203, 0x16);
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x7F, (int)chip8CPU.getRegisterV(0), L"Register 0 is incorrect", LINE_INFO());
			Assert::AreEqual((int)0x00, (int)chip8CPU.getRegisterV(0xF), L"Register F is incorrect", LINE_INFO());
			Assert::AreEqual((int)0x204, (int)chip8CPU.getRegisterPC(), L"PC Incorrect", LINE_INFO());
			chip8CPU.reset();
			// Set V0 to 0xFE
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x7F);
			// Call 0x8016
			memory.setValue(0x202, 0x80);
			memory.setValue(0x203, 0x16);
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x3F, (int)chip8CPU.getRegisterV(0), L"Register 0 is incorrect", LINE_INFO());
			Assert::AreEqual((int)0x01, (int)chip8CPU.getRegisterV(0xF), L"Register F is incorrect", LINE_INFO());
			Assert::AreEqual((int)0x204, (int)chip8CPU.getRegisterPC(), L"PC Incorrect", LINE_INFO());
		}

		/*
		// 0x8XY7
		// Set Vx = Vy - Vx, set VF = NOT borrow.
		// If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
		*/
		TEST_METHOD(_8XY7_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			// Set V0 to 0xFE
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0xFE);
			// Set V1 to 0x02
			memory.setValue(0x202, 0x61);
			memory.setValue(0x203, 0x02);
			// Run 0x8017
			memory.setValue(0x204, 0x80);
			memory.setValue(0x205, 0x17);
			chip8CPU.cycle();
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x04, (int)chip8CPU.getRegisterV(0), L"", LINE_INFO());
			Assert::AreEqual((int)0x02, (int)chip8CPU.getRegisterV(1), L"", LINE_INFO());
			Assert::AreEqual((int)0x00, (int)chip8CPU.getRegisterV(0xF), L"", LINE_INFO());
			chip8CPU.reset();
			// Set V0 to 0x05
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x05);
			// Set V1 to 0x06
			memory.setValue(0x202, 0x61);
			memory.setValue(0x203, 0x06);
			// Run 0x8017
			memory.setValue(0x204, 0x80);
			memory.setValue(0x205, 0x17);
			chip8CPU.cycle();
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x01, (int)chip8CPU.getRegisterV(0), L"", LINE_INFO());
			Assert::AreEqual((int)0x06, (int)chip8CPU.getRegisterV(1), L"", LINE_INFO());
			Assert::AreEqual((int)0x01, (int)chip8CPU.getRegisterV(0xF), L"", LINE_INFO());
		}

		TEST_METHOD(_8XYE_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			// Set V0 to 0xFE
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0xFE);
			// Run 0x801E
			memory.setValue(0x202, 0x80);
			memory.setValue(0x203, 0x1E);
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0xFC, (int)chip8CPU.getRegisterV(0), L"Register V0 is incorrect", LINE_INFO());
			Assert::AreEqual((int)0x01, (int)chip8CPU.getRegisterV(0xF), L"Register VF is incorrect", LINE_INFO());
			Assert::AreEqual((int)0x204, (int)chip8CPU.getRegisterPC(), L"PC Incorrect", LINE_INFO());
			chip8CPU.reset();
			// Set V0 to 0xFE
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x7E);
			// Run 0x801E
			memory.setValue(0x202, 0x80);
			memory.setValue(0x203, 0x1E);
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0xFC, (int)chip8CPU.getRegisterV(0), L"Register V0 is incorrect", LINE_INFO());
			Assert::AreEqual((int)0x00, (int)chip8CPU.getRegisterV(0xF), L"Register VF is incorrect", LINE_INFO());
			Assert::AreEqual((int)0x204, (int)chip8CPU.getRegisterPC(), L"PC Incorrect", LINE_INFO());
		}

		TEST_METHOD(_9XY0_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			// Set V0 to 0x35
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x35);
			// Set V1 to 0x22
			memory.setValue(0x202, 0x61);
			memory.setValue(0x203, 0x22);
			// Run command 0x9010
			memory.setValue(0x204, 0x90);
			memory.setValue(0x205, 0x10);
			chip8CPU.cycle();
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x208, (int)chip8CPU.getRegisterPC(), L"PC is incorrect when values are different", LINE_INFO());
			chip8CPU.reset();
			// Set V0 to 0x35
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x35);
			// Set V1 to 0x35
			memory.setValue(0x202, 0x61);
			memory.setValue(0x203, 0x35);
			// Run command 0x9010
			memory.setValue(0x204, 0x90);
			memory.setValue(0x205, 0x10);
			chip8CPU.cycle();
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x206, (int)chip8CPU.getRegisterPC(), L"PC is incorrect when values are the same", LINE_INFO());
		}

		TEST_METHOD(_ANNN_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			// set I to 0x535
			memory.setValue(0x200, 0xA5);
			memory.setValue(0x201, 0x35);
			chip8CPU.cycle();
			Assert::AreEqual((int)0x0535, (int)chip8CPU.getRegisterI(), L"Register I Incorrect", LINE_INFO());
			Assert::AreEqual((int)0x202, (int)chip8CPU.getRegisterPC(), L"PC Incorrect", LINE_INFO());
		}

		TEST_METHOD(_BNNN_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x53);
			memory.setValue(0x202, 0xB5);
			memory.setValue(0x203, 0x35);
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x53, (int)chip8CPU.getRegisterV(0), L"V0 is incorrect", LINE_INFO());
			Assert::AreEqual((int)0x588, (int)chip8CPU.getRegisterPC(), L"PC Incorrect", LINE_INFO());
		}

		TEST_METHOD(_CXNN_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			memory.setValue(0x200, 0xC0);
			memory.setValue(0x201, 0xBC);
			chip8CPU.cycle();
			char randNum = chip8CPU.currentRandomNumber();
			unsigned char val = randNum & 0xBC;
			Assert::AreEqual((int)val, (int)chip8CPU.getRegisterV(0), L"V0 is incorrect", LINE_INFO());
		}

		TEST_METHOD(_DXYN_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);

			// Manually setting memory to draw a pixel on the screen
			memory.setValue(0x450, 0xF0); // ****
			memory.setValue(0x451, 0x90); // *  *
			memory.setValue(0x452, 0x90); // *  *
			memory.setValue(0x453, 0x90); // *  *
			memory.setValue(0x454, 0xF0); // ****

			// Set I equal to start of memory location to draw sprite
			memory.setValue(0x200, 0xA4);
			memory.setValue(0x201, 0x50);
			chip8CPU.cycle();

			// Set V1 equal to the X coordinate
			memory.setValue(0x202, 0x61);
			memory.setValue(0x203, 0x00);
			chip8CPU.cycle();

			// Set V2 equal to the Y coordinate
			memory.setValue(0x204, 0x62);
			memory.setValue(0x205, 0x00);
			chip8CPU.cycle();

			// Call DXYN to tell Chip8 to load the bytes into graphics memory
			memory.setValue(0x206, 0xD1);
			memory.setValue(0x207, 0x25);
			chip8CPU.cycle();

			for (int i = 0; i < gfxMemory.MEMORY_SIZE; ++i)
			{
				if (i == 0 || i == 1 || i == 2 || i == 3 || i == 64 || i == 67 || i == 128 || i == 131 || i == 192 || i == 195 || i == 256 || i == 257 || i == 258 || i == 259)
				{
					Assert::AreEqual((int)1, (int)gfxMemory.getValue(i), L"gfxMemory not set correctly", LINE_INFO());
				}
				else {
					Assert::AreEqual((int)0, (int)gfxMemory.getValue(i), L"gfxMemory not set correctly", LINE_INFO());
				}
			}

			Assert::AreEqual((int)0x208, (int)chip8CPU.getRegisterPC(), L"PC value incorrect", LINE_INFO());
			Assert::IsTrue(chip8CPU.getDrawFlag(), L"Draw flag incorrect", LINE_INFO());
		}

		TEST_METHOD(_EX9E_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			// Set V1 to equal key position 0x00;
			memory.setValue(0x200, 0x61);
			memory.setValue(0x201, 0x00);
			chip8CPU.cycle();

			// Press key 0x00
			input.setKeyPressed(0x00, true);

			// Run 0xE19E
			memory.setValue(0x202, 0xE1);
			memory.setValue(0x203, 0x9E);
			chip8CPU.cycle();

			Assert::AreEqual((int)0x206, (int)chip8CPU.getRegisterPC(), L"PC counter incorrect", LINE_INFO());

			chip8CPU.reset();

			// Set V1 to equal key position 0x00;
			memory.setValue(0x200, 0x61);
			memory.setValue(0x201, 0x00);
			chip8CPU.cycle();

			// Press key 0x00
			input.setKeyPressed(0x00, false);

			// Run 0xE19E
			memory.setValue(0x202, 0xE1);
			memory.setValue(0x203, 0x9E);
			chip8CPU.cycle();

			Assert::AreEqual((int)0x204, (int)chip8CPU.getRegisterPC(), L"PC counter incorrect", LINE_INFO());

		}

		TEST_METHOD(_EXA1_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			// Set V1 to equal key position 0x00;
			memory.setValue(0x200, 0x61);
			memory.setValue(0x201, 0x00);
			chip8CPU.cycle();

			// Press key 0x00
			input.setKeyPressed(0x00, true);

			// Run 0xE19E
			memory.setValue(0x202, 0xE1);
			memory.setValue(0x203, 0xA1);
			chip8CPU.cycle();

			Assert::AreEqual((int)0x204, (int)chip8CPU.getRegisterPC(), L"PC counter incorrect when key is pressed.", LINE_INFO());

			chip8CPU.reset();

			// Set V1 to equal key position 0x00;
			memory.setValue(0x200, 0x61);
			memory.setValue(0x201, 0x00);
			chip8CPU.cycle();

			// Press key 0x00
			input.setKeyPressed(0x00, false);

			// Run 0xE19E
			memory.setValue(0x202, 0xE1);
			memory.setValue(0x203, 0xA1);
			chip8CPU.cycle();

			Assert::AreEqual((int)0x206, (int)chip8CPU.getRegisterPC(), L"PC counter incorrect when key is not pressed.", LINE_INFO());
		}

		TEST_METHOD(_FX07_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			// Set V0 to 0x32
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x32);
			// Call 0xF015 (Set DT to 0x32)
			memory.setValue(0x202, 0xF0);
			memory.setValue(0x203, 0x15);
			// Set V0 to 0xFE
			memory.setValue(0x204, 0x60);
			memory.setValue(0x205, 0xFE);
			// Call 0xF007
			memory.setValue(0x206, 0xF0);
			memory.setValue(0x207, 0x07);
			// Cycle CPU
			chip8CPU.cycle();
			chip8CPU.cycle();
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x30, (int)chip8CPU.getRegisterV(0), L"V0 incorrect", LINE_INFO());
			Assert::AreEqual((int)0x208, (int)chip8CPU.getRegisterPC(), L"PC incorrect", LINE_INFO());
		}

		/*
		// 0xFX0A
		// Wait for a key press, store the value of the key in Vx.
		// All execution stops until a key is pressed, then the value of that key is stored in Vx.
		*/
		TEST_METHOD(_FX0A_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);

			input.setKeyPressed(0x02, true);

			memory.setValue(0x200, 0xF1);
			memory.setValue(0x201, 0x0A);
			chip8CPU.cycle();
			
			Assert::AreEqual((int)0x202, (int)chip8CPU.getRegisterPC(), L"PC incorrect when key is pressed.", LINE_INFO());
			Assert::AreEqual((int)0x02, (int)chip8CPU.getRegisterV(0x01), L"V1 incorrect when key is pressed.", LINE_INFO());

			chip8CPU.reset();

			memory.setValue(0x200, 0xF1);
			memory.setValue(0x201, 0x0A);
			chip8CPU.cycle();

			Assert::AreEqual((int)0x200, (int)chip8CPU.getRegisterPC(), L"PC incorrect when key is not pressed.", LINE_INFO());
		}

		TEST_METHOD(_FX15_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			// Set V0 to 0x32
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x32);
			// Call 0xF015
			memory.setValue(0x202, 0xF0);
			memory.setValue(0x203, 0x15);
			// Cycle CPU
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x31, (int)chip8CPU.getRegisterDT(), L"DT is incorrect", LINE_INFO());
			Assert::AreEqual((int)0x204, (int)chip8CPU.getRegisterPC(), L"PC is incorrect", LINE_INFO());
		}

		TEST_METHOD(_FX18_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			// Set V0 to 0x32
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x32);
			// Call 0xFX18
			memory.setValue(0x202, 0xF0);
			memory.setValue(0x203, 0x18);
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x31, (int)chip8CPU.getRegisterST(), L"ST is incorrect", LINE_INFO());
			Assert::AreEqual((int)0x204, (int)chip8CPU.getRegisterPC(), L"PC is incorrect", LINE_INFO());
		}

		TEST_METHOD(_FX1E_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			// Set V0 to 0x32
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x32);
			// Set I to 0x23
			memory.setValue(0x202, 0xA0);
			memory.setValue(0x203, 0x23);
			// Call 0xF01E
			memory.setValue(0x204, 0xF0);
			memory.setValue(0x205, 0x1E);
			// Cycle CPU
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0x32, (int)chip8CPU.getRegisterV(0), L"Register V0 is incorrect", LINE_INFO());
			Assert::AreEqual((int)0x23, (int)chip8CPU.getRegisterI(), L"Register I is incorrect before 0xFX1E", LINE_INFO());
			chip8CPU.cycle();
			Assert::AreEqual((int)0x55, (int)chip8CPU.getRegisterI(), L"Register I is incorrect", LINE_INFO());
			Assert::AreEqual((int)0x206, (int)chip8CPU.getRegisterPC(), L"PC is incorrect", LINE_INFO());
		}

		TEST_METHOD(_FX29_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			// Set V0 to 0x32
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x32);
			// Call 0xFX29
			memory.setValue(0x202, 0xF0);
			memory.setValue(0x203, 0x29);
			chip8CPU.cycle();
			chip8CPU.cycle();
			Assert::AreEqual((int)0xFA, (int)chip8CPU.getRegisterI(), L"Register I is incorrect.", LINE_INFO());
			Assert::AreEqual((int)0x204, (int)chip8CPU.getRegisterPC(), L"PC is incorrect", LINE_INFO());
		}

		TEST_METHOD(_FX33_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			
			// Store 0x53 into V7
			memory.setValue(0x200, 0x67);
			memory.setValue(0x201, 0x53);
			chip8CPU.cycle();

			// Set I to 0x305
			memory.setValue(0x202, 0xA3);
			memory.setValue(0x203, 0x05);
			chip8CPU.cycle();

			// Call 0xFX33
			memory.setValue(0x204, 0xF7);
			memory.setValue(0x205, 0x33);
			chip8CPU.cycle();

			// Check memory at I
			Assert::AreEqual((int)0x00, (int)memory.getValue(0x305), L"I not correct.", LINE_INFO());

			// Check memory at I + 1
			Assert::AreEqual((int)0x08, (int)memory.getValue(0x306), L"I not correct.", LINE_INFO());

			// Check Memory at I + 2
			Assert::AreEqual((int)0x03, (int)memory.getValue(0x307), L"I not correct.", LINE_INFO());

			// Check PC
			Assert::AreEqual((int)0x206, (int)chip8CPU.getRegisterPC(), L"PC is incorrect", LINE_INFO());
		}

		TEST_METHOD(_FX55_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);
			// Set V0 to 0x35
			memory.setValue(0x200, 0x60);
			memory.setValue(0x201, 0x35);
			chip8CPU.cycle();
			// Set V1 to 0x43
			memory.setValue(0x202, 0x61);
			memory.setValue(0x203, 0x43);
			chip8CPU.cycle();
			// Set V2 to 0x99
			memory.setValue(0x204, 0x62);
			memory.setValue(0x205, 0x99);
			chip8CPU.cycle();
			// Set V3 to 0x23
			memory.setValue(0x206, 0x63);
			memory.setValue(0x207, 0x23);
			chip8CPU.cycle();
			// Set V4 to 0x19
			memory.setValue(0x208, 0x64);
			memory.setValue(0x209, 0x19);
			chip8CPU.cycle();
			// Set V5 to 0xAF
			memory.setValue(0x20A, 0x65);
			memory.setValue(0x20B, 0xAF);
			chip8CPU.cycle();
			// Set V6 to 0x76
			memory.setValue(0x20C, 0x66);
			memory.setValue(0x20D, 0x76);
			chip8CPU.cycle();
			// Set V7 to 0x89
			memory.setValue(0x20E, 0x67);
			memory.setValue(0x20F, 0x89);
			chip8CPU.cycle();
			// Set V8 to 0xFA
			memory.setValue(0x210, 0x68);
			memory.setValue(0x211, 0xFA);
			chip8CPU.cycle();
			// Set V9 to 0x77
			memory.setValue(0x212, 0x69);
			memory.setValue(0x213, 0x77);
			chip8CPU.cycle();
			// Set VA to 0xAA
			memory.setValue(0x214, 0x6A);
			memory.setValue(0x215, 0xAA);
			chip8CPU.cycle();
			// Set VB to 0xBB
			memory.setValue(0x216, 0x6B);
			memory.setValue(0x217, 0xBB);
			chip8CPU.cycle();
			// Set VC to 0xCC
			memory.setValue(0x218, 0x6C);
			memory.setValue(0x219, 0xCC);
			chip8CPU.cycle();
			// Set VD to 0xDD
			memory.setValue(0x21A, 0x6D);
			memory.setValue(0x21B, 0xDD);
			chip8CPU.cycle();
			// Set VE to 0xEE
			memory.setValue(0x21C, 0x6E);
			memory.setValue(0x21D, 0xEE);
			chip8CPU.cycle();
			// Set VF to 0xF4
			memory.setValue(0x21E, 0x6F);
			memory.setValue(0x21F, 0xFF);
			chip8CPU.cycle();
			// Set I to 0x304
			memory.setValue(0x220, 0xA3);
			memory.setValue(0x221, 0x04);
			chip8CPU.cycle();
			// Call 0xFX55
			memory.setValue(0x222, 0xFF);
			memory.setValue(0x223, 0x55);
			chip8CPU.cycle();
			Assert::AreEqual((int)0x35, (int)chip8CPU.getRegisterV(0), L"V0 not correct.", LINE_INFO());
			Assert::AreEqual((int)0x43, (int)chip8CPU.getRegisterV(1), L"V1 not correct.", LINE_INFO());
			Assert::AreEqual((int)0x99, (int)chip8CPU.getRegisterV(2), L"V2 not correct.", LINE_INFO());
			Assert::AreEqual((int)0x23, (int)chip8CPU.getRegisterV(3), L"V3 not correct.", LINE_INFO());
			Assert::AreEqual((int)0x19, (int)chip8CPU.getRegisterV(4), L"V4 not correct.", LINE_INFO());
			Assert::AreEqual((int)0xAF, (int)chip8CPU.getRegisterV(5), L"V5 not correct.", LINE_INFO());
			Assert::AreEqual((int)0x76, (int)chip8CPU.getRegisterV(6), L"V6 not correct.", LINE_INFO());
			Assert::AreEqual((int)0x89, (int)chip8CPU.getRegisterV(7), L"V7 not correct.", LINE_INFO());
			Assert::AreEqual((int)0xFA, (int)chip8CPU.getRegisterV(8), L"V8 not correct.", LINE_INFO());
			Assert::AreEqual((int)0x77, (int)chip8CPU.getRegisterV(9), L"V9 not correct.", LINE_INFO());
			Assert::AreEqual((int)0xAA, (int)chip8CPU.getRegisterV(0xA), L"VA not correct.", LINE_INFO());
			Assert::AreEqual((int)0xBB, (int)chip8CPU.getRegisterV(0xB), L"VB not correct.", LINE_INFO());
			Assert::AreEqual((int)0xCC, (int)chip8CPU.getRegisterV(0xC), L"VC not correct.", LINE_INFO());
			Assert::AreEqual((int)0xDD, (int)chip8CPU.getRegisterV(0xD), L"VD not correct.", LINE_INFO());
			Assert::AreEqual((int)0xEE, (int)chip8CPU.getRegisterV(0xE), L"VE not correct.", LINE_INFO());
			Assert::AreEqual((int)0xFF, (int)chip8CPU.getRegisterV(0xF), L"VF not correct.", LINE_INFO());
			Assert::AreEqual((int)0x314, (int)chip8CPU.getRegisterI(), L"I not correct.", LINE_INFO());
			Assert::AreEqual((int)0x224, (int)chip8CPU.getRegisterPC(), L"PC not correct.", LINE_INFO());
			Assert::AreEqual((int)0x35, (int)memory.getValue(0x304), L"Memory 0x304 not correct", LINE_INFO());
			Assert::AreEqual((int)0x43, (int)memory.getValue(0x305), L"Memory 0x305 not correct", LINE_INFO());
			Assert::AreEqual((int)0x99, (int)memory.getValue(0x306), L"Memory 0x306 not correct", LINE_INFO());
			Assert::AreEqual((int)0x23, (int)memory.getValue(0x307), L"Memory 0x307 not correct", LINE_INFO());
			Assert::AreEqual((int)0x19, (int)memory.getValue(0x308), L"Memory 0x308 not correct", LINE_INFO());
			Assert::AreEqual((int)0xAF, (int)memory.getValue(0x309), L"Memory 0x309 not correct", LINE_INFO());
			Assert::AreEqual((int)0x76, (int)memory.getValue(0x30A), L"Memory 0x30A not correct", LINE_INFO());
			Assert::AreEqual((int)0x89, (int)memory.getValue(0x30B), L"Memory 0x30B not correct", LINE_INFO());
			Assert::AreEqual((int)0xFA, (int)memory.getValue(0x30C), L"Memory 0x30C not correct", LINE_INFO());
			Assert::AreEqual((int)0x77, (int)memory.getValue(0x30D), L"Memory 0x30D not correct", LINE_INFO());
			Assert::AreEqual((int)0xAA, (int)memory.getValue(0x30E), L"Memory 0x30E not correct", LINE_INFO());
			Assert::AreEqual((int)0xBB, (int)memory.getValue(0x30F), L"Memory 0x30F not correct", LINE_INFO());
			Assert::AreEqual((int)0xCC, (int)memory.getValue(0x310), L"Memory 0x310 not correct", LINE_INFO());
			Assert::AreEqual((int)0xDD, (int)memory.getValue(0x311), L"Memory 0x311 not correct", LINE_INFO());
			Assert::AreEqual((int)0xEE, (int)memory.getValue(0x312), L"Memory 0x312 not correct", LINE_INFO());
			Assert::AreEqual((int)0xFF, (int)memory.getValue(0x313), L"Memory 0x313 not correct", LINE_INFO());
		}

		TEST_METHOD(_FX65_Test)
		{
			Chip8Memory memory;
			Chip8GraphicsMemory gfxMemory;
			Chip8Input input;
			Chip8CPU chip8CPU(&memory, &gfxMemory, &input);

			// Fill Memory with 16 values
			memory.setValue(0x304, 0x35);
			memory.setValue(0x305, 0x43);
			memory.setValue(0x306, 0x99);
			memory.setValue(0x307, 0x23);
			memory.setValue(0x308, 0x19);
			memory.setValue(0x309, 0xAF);
			memory.setValue(0x30A, 0x76);
			memory.setValue(0x30B, 0x89);
			memory.setValue(0x30C, 0xFA);
			memory.setValue(0x30D, 0x77);
			memory.setValue(0x30E, 0xAA);
			memory.setValue(0x30F, 0xBB);
			memory.setValue(0x310, 0xCC);
			memory.setValue(0x311, 0xDD);
			memory.setValue(0x312, 0xEE);
			memory.setValue(0x313, 0xFF);


			// Set I to 0x304
			memory.setValue(0x200, 0xA3);
			memory.setValue(0x201, 0x04);
			chip8CPU.cycle();

			// Call 0xFX65
			memory.setValue(0x202, 0xFF);
			memory.setValue(0x203, 0x65);
			chip8CPU.cycle();

			// Check values of V0 - VF
			Assert::AreEqual((int)0x35, (int)chip8CPU.getRegisterV(0), L"V0 not correct.", LINE_INFO());
			Assert::AreEqual((int)0x43, (int)chip8CPU.getRegisterV(1), L"V1 not correct.", LINE_INFO());
			Assert::AreEqual((int)0x99, (int)chip8CPU.getRegisterV(2), L"V2 not correct.", LINE_INFO());
			Assert::AreEqual((int)0x23, (int)chip8CPU.getRegisterV(3), L"V3 not correct.", LINE_INFO());
			Assert::AreEqual((int)0x19, (int)chip8CPU.getRegisterV(4), L"V4 not correct.", LINE_INFO());
			Assert::AreEqual((int)0xAF, (int)chip8CPU.getRegisterV(5), L"V5 not correct.", LINE_INFO());
			Assert::AreEqual((int)0x76, (int)chip8CPU.getRegisterV(6), L"V6 not correct.", LINE_INFO());
			Assert::AreEqual((int)0x89, (int)chip8CPU.getRegisterV(7), L"V7 not correct.", LINE_INFO());
			Assert::AreEqual((int)0xFA, (int)chip8CPU.getRegisterV(8), L"V8 not correct.", LINE_INFO());
			Assert::AreEqual((int)0x77, (int)chip8CPU.getRegisterV(9), L"V9 not correct.", LINE_INFO());
			Assert::AreEqual((int)0xAA, (int)chip8CPU.getRegisterV(0xA), L"VA not correct.", LINE_INFO());
			Assert::AreEqual((int)0xBB, (int)chip8CPU.getRegisterV(0xB), L"VB not correct.", LINE_INFO());
			Assert::AreEqual((int)0xCC, (int)chip8CPU.getRegisterV(0xC), L"VC not correct.", LINE_INFO());
			Assert::AreEqual((int)0xDD, (int)chip8CPU.getRegisterV(0xD), L"VD not correct.", LINE_INFO());
			Assert::AreEqual((int)0xEE, (int)chip8CPU.getRegisterV(0xE), L"VE not correct.", LINE_INFO());
			Assert::AreEqual((int)0xFF, (int)chip8CPU.getRegisterV(0xF), L"VF not correct.", LINE_INFO());
			
			// Check value of I
			Assert::AreEqual((int)0x314, (int)chip8CPU.getRegisterI(), L"I Incorrect", LINE_INFO());

			// Check value of PC
			Assert::AreEqual((int)0x204, (int)chip8CPU.getRegisterPC(), L"PC Incorrect", LINE_INFO());
		}
	};

	TEST_CLASS(Chip8InputTests)
	{
	public:
		TEST_METHOD(SetAndGetKeyPressedTest)
		{
			Chip8Input input;
			for (int i = 0; i < 16; i++)
			{
				Assert::IsFalse(input.getKeyPressed(i), L"A Key was not set to false at initialization.", LINE_INFO());
			}
			for (int i = 0; i < 16; i++)
			{
				input.setKeyPressed(i, true);
				Assert::IsTrue(input.getKeyPressed(i), L"A Key was not set properly.", LINE_INFO());
			}
		}

		TEST_METHOD(ClearKeysTest)
		{
			Chip8Input input;
			for (int i = 0; i < 16; i++)
			{
				input.setKeyPressed(i, true);
				Assert::IsTrue(input.getKeyPressed(i), L"A Key was not set properly.", LINE_INFO());
			}
			input.clearKeys();
			for (int i = 0; i < 16; i++)
			{
				Assert::IsFalse(input.getKeyPressed(i), L"A Key was not set to false during clearKeys().", LINE_INFO());
			}
		}
	};

	TEST_CLASS(Chip8GraphicsMemoryTests)
	{
	public:
		TEST_METHOD(GetAndSetValueTest)
		{
			Chip8GraphicsMemory memory;
			unsigned char targetVal = 1;
			unsigned short memLocation = 0x001;
			memory.setValue(memLocation, targetVal);
			unsigned char val = memory.getValue(memLocation);
			Assert::AreEqual(targetVal, val, L"Memory value that was set does not match what was retrieved.", LINE_INFO());
		}

		TEST_METHOD(ClearMemoryTest)
		{
			Chip8GraphicsMemory memory;
			unsigned short memLocation = 0x020;
			unsigned char val = 5;
			memory.setValue(memLocation, val);
			memory.clear();
			val = memory.getValue(memLocation);
			unsigned char targetVal = 0;
			Assert::AreEqual(targetVal, val, L"Memory not cleared.", LINE_INFO());
		}
	};

	TEST_CLASS(Chip8MemoryTests)
	{
	public:

		TEST_METHOD(GetAndSetValueTest)
		{
			Chip8Memory memory;
			unsigned char targetVal = 1;
			unsigned short memLocation = 0x001;
			memory.setValue(memLocation, targetVal);
			unsigned char val = memory.getValue(memLocation);
			Assert::AreEqual(targetVal, val, L"Memory value that was set does not match what was retrieved.", LINE_INFO());
		}

		TEST_METHOD(GetValueOutOfRangeTest)
		{
			bool exceptionThrown = false;
			Chip8Memory memory;
			try
			{
				memory.getValue(5000);
			}
			catch (Chip8MemoryException::out_of_range& ex)
			{
				exceptionThrown = true;
			}
			Assert::IsTrue(exceptionThrown, L"Exception was not thrown when value requested was out of range", LINE_INFO());
			exceptionThrown = false;
			try
			{
				memory.getValue(-5);
			}
			catch (Chip8MemoryException::out_of_range& ex)
			{
				exceptionThrown = true;
			}
			Assert::IsTrue(exceptionThrown, L"Exception was not thrown when value requested was out of range", LINE_INFO());
		}

		TEST_METHOD(SetValueOutOfRangeTest)
		{
			bool exceptionThrown = false;
			Chip8Memory memory;
			try
			{
				memory.setValue(4096, 5);
			}
			catch (Chip8MemoryException::out_of_range& ex)
			{
				exceptionThrown = true;
			}
			Assert::IsTrue(exceptionThrown);
			exceptionThrown = false;
			try
			{
				memory.setValue(-5, 5);
			}
			catch (Chip8MemoryException::out_of_range& ex)
			{
				exceptionThrown = true;
			}
			Assert::IsTrue(exceptionThrown);
		}

		TEST_METHOD(IsClearTest)
		{
			Chip8Memory memory;
			memory.clear();
			Assert::IsTrue(memory.isClear(), L"Memory Clear did not return True when no values existed in memory.", LINE_INFO());
			memory.setValue(0x020, (unsigned char)5);
			Assert::IsFalse(memory.isClear(), L"Memory Clear did not return False when values existed in memory.", LINE_INFO());
		}

		TEST_METHOD(ClearMemoryTest)
		{
			Chip8Memory memory;
			unsigned short memLocation = 0x020;
			unsigned char val = 5;
			memory.setValue(memLocation, val);
			memory.clear();
			val = memory.getValue(memLocation);
			unsigned char targetVal = 0;
			Assert::AreEqual(targetVal, val, L"Memory not cleared.", LINE_INFO());
		}
	};
}