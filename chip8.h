#pragma once

#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>

class Chip8
{
public:
    //RAM (4096 Bytes)
    uint8_t memory[4096];

    //16 Registers V0,V1,....VF
    uint8_t V[16];
    //Special Register Used to Store memory Addresses
    uint16_t I;
    //Two 8-bit registers used for delay and Sound timer
    uint8_t delayTimer, soundTimer;
    //Program Counter
    uint16_t PC;

    //Stack Pointer
    uint8_t stackPointer;
    //Stack itself
    uint16_t stack[16];

    //input output devices
    bool keyboard[16];
    bool display[64*32];

    Chip8();
    void loadROM(const char* filename);
    void cycle();

    uint16_t fetch();
    void decode(uint16_t opcode);
    void execute();
};