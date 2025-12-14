#include <SDL3/SDL.h>
#include "chip8.h"

const int width = 640;
const int height = 320;
const int cell_width = width / 64;
const int cell_height = height / 32;

void handle_input(Chip8 &cpu);

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("CHIP-8", width, height, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);

    Chip8 cpu;
    bool running = true;
    SDL_Event event;

    const SDL_FRect rect
    {
        .x = 0, .y = 0, .w = width, .h = height
    };

    while(running)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        //update screen
        //handle input
        handle_input(cpu);
        //update timers
        //cpu cycle
        cpu.cycle();

        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);
        SDL_Delay(20);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void handle_input(Chip8 &cpu)
{
    const bool *keys = SDL_GetKeyboardState(nullptr);
    memset(cpu.keyboard, false, sizeof(cpu.keyboard));

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
}