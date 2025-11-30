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
    void execute(uint16_t opcode);

private:
    void CLS();
    void RET();
    void JP_addr();
    void CALL_addr();
    void SE_Vx_kk();
    void SNE_Vx_kk();
    void SE_Vx_Vy();
    void LD_Vx_kk();
    void ADD_Vx_kk();
    void LD_Vx_Vy();
    void OR_Vx_Vy();
    void AND_Vx_Vy();
    void XOR_Vx_Vy();
    void ADD_Vx_Vy();
    void SUB_Vx_Vy();
    void SHR_Vx_Vy();
    void SUBN_Vx_Vy();
    void SHL_Vx_Vy();
    void SNE_Vx_Vy();
    void LD_I_addr();
    void JP_V0_addr();
    void RND_Vx_kk();
    void DRW_Vx_Vy_n();
    void SKP_Vx();
    void SKNP_Vx();
    void LD_Vx_dt();
    void LD_Vx_k();
    void LD_dt_Vx();
    void LD_st_Vx();
    void ADD_I_Vx();
    void LD_f_Vx();
    void LD_b_Vx();
    void LD_I_Vx();
    void LD_Vx_I();
};