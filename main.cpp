#include <SDL3/SDL.h>
#include "chip8.h"

const int width = 640;
const int height = 320;
const int cell_width = width / 64;
const int cell_height = height / 32;

void handle_input(Chip8 &cpu);
void update_screen(Chip8 &cpu, SDL_Renderer *renderer);

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <rom_name.ch8>\n";
        return -1;
    }

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("CHIP-8", width, height, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);

    Chip8 cpu;
    cpu.loadROM(argv[1]);

    bool running = true;
    SDL_Event event;
    bool wasBeeping = false;

    uint32_t last_cpu_tick = SDL_GetTicks();
    uint32_t last_timer_tick = SDL_GetTicks();
    uint32_t current_tick;

    while(running)
    {
        current_tick = SDL_GetTicks();
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        handle_input(cpu);
        
        //cpu cycle and timers
        if(current_tick - last_cpu_tick >= 1) //1000Hz
        {
            cpu.cycle();
            last_cpu_tick = current_tick;
        }
        if(current_tick - last_timer_tick >= 16) //60Hz
        {
            cpu.update_timers();
            last_timer_tick = current_tick;
        }

        //update screen and handle key press and play sound
        if(cpu.draw_flag)
        {
            update_screen(cpu, renderer);
            cpu.draw_flag = false;
        }

        if(cpu.soundTimer > 0 && !wasBeeping)
        {
            std::cout<< "\a\a" << std::flush;
        }
        wasBeeping = (cpu.soundTimer > 0);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void handle_input(Chip8& cpu)
{
    memcpy(cpu.prev_keyboard, cpu.keyboard, sizeof(cpu.keyboard));
    memset(cpu.keyboard, false, sizeof(cpu.keyboard));

    const bool* keys = SDL_GetKeyboardState(nullptr);

    cpu.keyboard[0x1] = keys[SDL_SCANCODE_1];
    cpu.keyboard[0x2] = keys[SDL_SCANCODE_2];
    cpu.keyboard[0x3] = keys[SDL_SCANCODE_3];
    cpu.keyboard[0xC] = keys[SDL_SCANCODE_4];

    cpu.keyboard[0x4] = keys[SDL_SCANCODE_Q];
    cpu.keyboard[0x5] = keys[SDL_SCANCODE_W];
    cpu.keyboard[0x6] = keys[SDL_SCANCODE_E];
    cpu.keyboard[0xD] = keys[SDL_SCANCODE_R];

    cpu.keyboard[0x7] = keys[SDL_SCANCODE_A];
    cpu.keyboard[0x8] = keys[SDL_SCANCODE_S];
    cpu.keyboard[0x9] = keys[SDL_SCANCODE_D];
    cpu.keyboard[0xE] = keys[SDL_SCANCODE_F];

    cpu.keyboard[0xA] = keys[SDL_SCANCODE_Z];
    cpu.keyboard[0x0] = keys[SDL_SCANCODE_X];
    cpu.keyboard[0xB] = keys[SDL_SCANCODE_C];
    cpu.keyboard[0xF] = keys[SDL_SCANCODE_V];

    // latch new key presses
    for (int i = 0; i < 16; i++)
    {
        if (cpu.keyboard[i] && !cpu.prev_keyboard[i])
            cpu.latched_key = i;
    }
}

void update_screen(Chip8 &cpu, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255,255,255,255);

    for(int y = 0; y < 32; ++y)
    {
        for(int x = 0; x < 64; ++x)
        {
            if(cpu.display[y * 64 + x])
            {
                SDL_FRect pixel
                    {
                        .x = (float) x * cell_width,
                        .y = (float) y * cell_height,
                        .w = cell_width,
                        .h = cell_height
                    };
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }
    SDL_RenderPresent(renderer);
}