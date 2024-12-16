// build using :
//  g++ main.cpp -o prog -lmingw32 -lSDL2main -lSDL2

// C++ Standard libraries
#include <iostream>

// SDL Standard libraries
#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
    // Returns the SDL_Window that was created or NULL on failure
    SDL_Window *window = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL couldn't be initialized" << SDL_GetError();
    }
    else
    {
        std::cout << "SDL video system is ready!";
    }

    window = SDL_CreateWindow("This is the title of the window",
                              110, 110, 640,
                              480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    SDL_Delay(30000);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}