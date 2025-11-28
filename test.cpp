#include"chip8.h"
#include<iostream>
#include <iomanip>

int main()
{
    Chip8 cpu;
    cpu.loadROM("1-chip8-logo.ch8");
    for(int i = 512; i < 512 + 20; ++i)
    {
        std::cout << std::hex 
              << std::setw(2)          // Always width of 2
              << std::setfill('0')     // Fill empty space with '0'
              << (int)cpu.memory[i] 
              << " ";                  // Add a space for readability
    }
    return 0;
}