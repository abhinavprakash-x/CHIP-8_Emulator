#include"chip8.h"
#include<iostream>
#include <iomanip>

int main()
{
    Chip8 cpu;
    cpu.loadROM("5-quirks.ch8");
    for(int i = 512; i < 4096; ++i)
    {
        std::cout << std::hex 
              << std::setw(2)          // Always width of 2
              << std::setfill('0')     // Fill empty space with '0'
              << (int)cpu.memory[i] 
              << " ";                  // Add a space for readability
    }
    std::cout<<"\n\n\n";
    cpu.cycle();
    return 0;
}