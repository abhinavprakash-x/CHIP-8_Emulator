#include "chip8.h"

const uint8_t fontset[80] = {
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

Chip8::Chip8()
{
    memset(memory, 0, sizeof(memory));
    memset(V, 0, sizeof(V));
    I = 0;
    delayTimer = 0;
    soundTimer = 0;
    PC = 512;
    stackPointer = 0;
    memset(stack, 0, sizeof(stack));
    memset(keyboard, false, sizeof(keyboard));
    memset(display, false, sizeof(display));

    for(int i = 0; i < 80; ++i)
    {
        memory[i] = fontset[i];
    }
}

void Chip8::loadROM(const char* filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (file.is_open())
    {
        std::streamsize size = file.tellg(); //tellg() returns the current position = no. of bytes in file
        file.seekg(0, std::ios::beg); //seekg() moves back to start of file

        // Safety: Don't overflow the Chip8 memory
        if (size > (4096 - 512))
        {
            std::cout << "ROM too large to fit in memory!" << std::endl;
            return;
        }

        // Read directly into memory array at index 512
        // We cast to char* because fstream expects char*, but memory is uint8_t
        file.read(reinterpret_cast<char*>(&memory[512]), size);
        file.close();
    }
}

void Chip8::cycle()
{
    while(true)
    {
        uint16_t opcode = fetch();
        decode(opcode);
        execute();
    }
}

uint16_t Chip8::fetch()
{
    uint16_t opcode = 0;
    opcode = (memory[PC] << 8) | memory[PC+1];
    PC = PC+2;
    return opcode;
}

void Chip8::decode(uint16_t opcode)
{
}

void Chip8::execute()
{
}