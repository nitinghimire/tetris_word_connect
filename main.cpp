// build using :
//  g++ main.cpp glad/src/glad.c -I./glad/include -o prog -lmingw32 -lSDL2main -lSDL2

// C++ Standard libraries
#include <iostream>

// SDL Standard libraries
#include <SDL2/SDL.h>

// include the glad header files
#include <glad\glad.h>

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

    // specifying the opengl version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow("Tetris Word Game",
                              110, 110, 640,
                              480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    SDL_GLContext context;
    context = SDL_GL_CreateContext(window);

    // loading the functions from glad
    gladLoadGLLoader(SDL_GL_GetProcAddress);

    // Infinite loop for our game
    bool whileGameIsRunning = true;
    while (whileGameIsRunning)
    {
        SDL_Event event;
        // start event loop
        while (SDL_PollEvent(&event))
        {
            // This probably where we will handle the mouse and keyboard events

            if (event.type == SDL_QUIT)
            {
                whileGameIsRunning = false;
            }

            if (event.type == SDL_MOUSEMOTION)
            {
                std::cout << "mouse is moved";
            }

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_0)
                {
                    std::cout << "The 0 key was pressed";
                }
                else
                {
                    std::cout << "Another key was pressed";
                }

                std::cout << "key is moved";
            }

            const Uint8 *state = SDL_GetKeyboardState(NULL);
            if (state[SDL_SCANCODE_RIGHT])
            {
                std::cout << "Right arrow key was pressed";
            }
        }

        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}