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
    uint16_t opcode = fetch();
    execute(opcode);
}

uint16_t Chip8::fetch()
{
    uint16_t opcode = (memory[PC] << 8) | memory[PC+1];
    PC = PC+2;
    return opcode;
}

void Chip8::execute(uint16_t opcode)
{
    //first decode x,y,nnn,n,kk from opcode and then execute
    uint16_t NNN = opcode & 0x0fff;
    uint8_t KK = opcode & 0x00ff;
    uint8_t N = opcode & 0x000f;
    uint8_t X = (opcode & 0x0f00) >> 8;
    uint8_t Y = (opcode & 0x00f0) >> 4;

    //now execute
    uint16_t first_bit = opcode & 0xf000;
    switch(first_bit)
    {
        case 0x0000:
        {
            if(opcode == 0x00e0) CLS();
            else if(opcode == 0x00ee) RET();
            break;
        }
        case 0x1000: JP_addr(NNN); break;
        case 0x2000: CALL_addr(NNN); break;
        case 0x3000: SE_Vx_kk(); break;
        case 0x4000: SNE_Vx_kk(); break;
        case 0x5000: SE_Vx_Vy(); break;
        case 0x6000: LD_Vx_kk(); break;
        case 0x7000: ADD_Vx_kk(); break;
        case 0x8000:
        {
            uint8_t last_bit = opcode & 0x000f;
            if(last_bit == 0x00) LD_Vx_Vy();
            else if(last_bit == 0x01) OR_Vx_Vy();
            else if(last_bit == 0x02) AND_Vx_Vy();
            else if(last_bit == 0x03) XOR_Vx_Vy();
            else if(last_bit == 0x04) ADD_Vx_Vy();
            else if(last_bit == 0x05) SUB_Vx_Vy();
            else if(last_bit == 0x06) SHR_Vx_Vy();
            else if(last_bit == 0x07) SUBN_Vx_Vy();
            else if(last_bit == 0x0e) SHL_Vx_Vy();
            else std::cout<<"Error! last 4 bits of opcode in 8xy_ is wrong. \n";
            break;
        }
        case 0x9000: SNE_Vx_Vy(); break;
        case 0xa000: LD_I_addr(); break;
        case 0xb000: JP_V0_addr(); break;
        case 0xc000: RND_Vx_kk(); break;
        case 0xd000: DRW_Vx_Vy_n(); break;
        case 0xe000:
        {
            uint8_t last_bits = opcode & 0xff;
            if(last_bits == 0x9e) SKP_Vx();
            else if(last_bits == 0xa1) SKNP_Vx();
            else std::cout<<"Error! last 8 bits of opcode in Ex__ is wrong. \n";
            break;
        }
        case 0xf000:
        {
            uint8_t last_bits = opcode & 0xff;
            if(last_bits == 0x07) LD_Vx_dt();
            else if(last_bits == 0x0a) LD_Vx_k();
            else if(last_bits == 0x15) LD_dt_Vx();
            else if(last_bits == 0x18) LD_st_Vx();
            else if(last_bits == 0x1e) ADD_I_Vx();
            else if(last_bits == 0x29) LD_f_Vx();
            else if(last_bits == 0x33) LD_b_Vx();
            else if(last_bits == 0x55) LD_I_Vx();
            else if(last_bits == 0x65) LD_Vx_I();
            else std::cout<<"Error! last 8 bits of opcode in Fx__ is wrong. \n";
            break;
        }
        default: std::cout<<"Error with opcode! \n"; //also cout the opcode TODO!
    }
}

void Chip8::CLS()
{
    memset(display, false, sizeof(display));
}

void Chip8::RET()
{
    if(stackPointer == 0)
    {
        std::cout<<"Error! Invalid Stack Operation (Underflow)";
        return;
    }
    stackPointer--;
    PC = stack[stackPointer];
}

void Chip8::JP_addr(uint16_t NNN)
{
    PC = NNN;
}

void Chip8::CALL_addr(uint16_t NNN)
{
    if(stackPointer >= 16)
    {
        std::cout<<"Error! Invalid Stack Operation (Overflow)";
        return;
    }
    stack[stackPointer] = PC;
    stackPointer++;
    PC = NNN;
}