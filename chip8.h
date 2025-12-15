#pragma once

#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>

class Chip8
{
public:

    //The Chip8 Core Architecture
    uint8_t memory[4096];               //RAM 4KB
    uint8_t V[16];                      //V0-VF Registers
    uint16_t I;                         //Index Register
    uint8_t delayTimer, soundTimer;     //60Hz Timers
    uint16_t PC;                        //Program Counter
    uint8_t stackPointer;               //Stack Pointer
    uint16_t stack[16];                 //Stack

    //Input Output Device States
    bool keyboard[16];
    bool prev_keyboard[16];
    bool display[64*32];
    
    int latched_key;         //Fix for Edge vs Level Trigger Key Press
    bool draw_flag;         //draw flag for optimization

    //Public Functions
    Chip8();
    void loadROM(const char* filename);
    void cycle();
    void update_timers();

    //Fetch Decode Execute Cycle
    uint16_t fetch();
    void execute(uint16_t opcode);

private:

    //Private Functions, Implementations of all opcodes
    void CLS();
    void RET();
    void JP_addr(uint16_t NNN);
    void CALL_addr(uint16_t NNN);
    void SE_Vx_kk(uint8_t X, uint8_t KK); 
    void SNE_Vx_kk(uint8_t X, uint8_t KK);
    void SE_Vx_Vy(uint8_t X, uint8_t Y);  
    void LD_Vx_kk(uint8_t X, uint8_t KK); 
    void ADD_Vx_kk(uint8_t X, uint8_t KK);
    void LD_Vx_Vy(uint8_t X, uint8_t Y);
    void OR_Vx_Vy(uint8_t X, uint8_t Y);
    void AND_Vx_Vy(uint8_t X, uint8_t Y);
    void XOR_Vx_Vy(uint8_t X, uint8_t Y);
    void ADD_Vx_Vy(uint8_t X, uint8_t Y);
    void SUB_Vx_Vy(uint8_t X, uint8_t Y);
    void SHR_Vx_Vy(uint8_t X, uint8_t Y);
    void SUBN_Vx_Vy(uint8_t X, uint8_t Y);
    void SHL_Vx_Vy(uint8_t X, uint8_t Y);
    void SNE_Vx_Vy(uint8_t X, uint8_t Y);
    void LD_I_addr(uint16_t NNN);
    void JP_V0_addr(uint16_t NNN);
    void RND_Vx_kk(uint8_t X, uint8_t KK);
    void DRW_Vx_Vy_n(uint8_t X, uint8_t Y, uint8_t N);
    void SKP_Vx(uint8_t X);
    void SKNP_Vx(uint8_t X);
    void LD_Vx_dt(uint8_t X);
    void LD_Vx_k(uint8_t X);
    void LD_dt_Vx(uint8_t X);
    void LD_st_Vx(uint8_t X);
    void ADD_I_Vx(uint8_t X);
    void LD_f_Vx(uint8_t X);
    void LD_b_Vx(uint8_t X);
    void LD_I_Vx(uint8_t X);
    void LD_Vx_I(uint8_t X);
};