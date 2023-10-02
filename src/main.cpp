#include "../include/game.hpp"
#include "../include/sdl2/SDL.h"
#include <iostream>

int main(int argc, char *argcv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    spawnCells();

    // Loop variables
    bool stop = false;
    SDL_Event event;

    while (!stop)
    {
        // Set background to clear with to transparent
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
        // Clears previous frame
        SDL_RenderClear(renderer);

        // Handle events on queue
        while (SDL_PollEvent(&event) != 0)
        {
            // Handle quiting
            if (event.type == SDL_QUIT)
            {
                stop = true;
            }

            startTurn(event);
        }

        // Set background to display to dark grey
        SDL_SetRenderDrawColor(renderer, 220, 220, 220, SDL_ALPHA_OPAQUE);

        // Render all buttons to backbuffer
        renderAll();

        // Update screen with renders from backbuffer
        SDL_RenderPresent(renderer);

        // Slow down program
        SDL_Delay(10);
    }

    // Destroy renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit
    SDL_Quit();

    return 0;
}