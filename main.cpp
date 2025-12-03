#include <SDL3/SDL.h>
#include "chip8.h"

const int width = 640;
const int height = 320;
const int cell_width = width / 64;
const int cell_height = height / 32;

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("CHIP-8", width, height, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);

    //Chip8 cpu;
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

        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}