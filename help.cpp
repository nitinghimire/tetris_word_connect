// build command: g++ game.cpp -o game.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer

// include the necessary libraries

#include <iostream>

// Include the sdl library
#include <SDL2\SDL.h>
// include the image library and also the mixer library
#include <SDL2\SDL_image.h>
// include a mixer library to add music
#include <SDL2\SDL_mixer.h>

// library for random number generation
#include <bits/stdc++.h>

// import string library for filepath
#include <string>
#include <vector>
using namespace std;

// now lets define some functions that seperate the functionality of the game

// Lets start by defining the initization part of the function called init()
bool init();

// The next part of the function lets call it main_game_loop()
bool main_game_loop();

// lets create a load function that creates the blocks and

// Finally the final part of the program, where we close all the contexts and properly exit the program
void end();

// Now lets define some important contexts and pointers to various objects
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
SDL_Texture *block_texture;
SDL_Texture *main_texture;

Mix_Music *music;
Mix_Chunk *sound;

struct BlockData
{
    SDL_Texture *texture;
    SDL_Rect rect; // To store position and size
};
// BlockData *texture_rect_list[24];
std::vector<BlockData> blocks; // Now a vector of BlockData

// define a state for the game to use to represent that the object is currently falling
static bool object_falling = false;

static int texture_index = 0;
// grid to store character values
const int ROWS = 6;
const int COLS = 4;
char grid[ROWS][COLS] = {{'\0'}};

// lets define the main function now
int main(int argc, char **args)
{
    if (!init())
    {
        return 1;
    }

    while (main_game_loop())
    {
        SDL_Delay(10);
    }

    end();
    return 0;
}

// This is the function that initializes the sdl library various contexts like SDL_Window, SDL_Renderer
bool init()
{

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        cout << "Error initializing SDL: " << SDL_GetError() << endl;
        system("pause");
        return false;
    }

    window = SDL_CreateWindow("Tetris:Word Game", 500, 100, 400, 600, SDL_WINDOW_SHOWN);
    if (!window)
    {
        cout << "Error creating window: " << SDL_GetError() << endl;
        system("pause");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        cout << "Error creating renderer: " << SDL_GetError() << endl;
        return false;
    }

    // Load bitmap into surface
    SDL_Surface *buffer = SDL_LoadBMP("assets\\tetris-background.bmp");
    if (!buffer)
    {
        cout << "Error loading image tetris-background.bmp: " << SDL_GetError() << endl;
        return false;
    }

    // Create texture
    texture = SDL_CreateTextureFromSurface(renderer, buffer);
    // Free surface as it's no longer needed
    SDL_FreeSurface(buffer);
    buffer = NULL;
    if (!texture)
    {
        cout << "Error creating texture: " << SDL_GetError() << endl;
        return false;
    }

    int result = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

    // Check load
    if (result == 0)
    {
        cout << "Failed to initialize SDL_image: " << IMG_GetError() << endl;
    }

    cout << "SDL_image initialized successfully!" << endl;

    main_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 400, 600);
    SDL_RenderClear(renderer);

    int audio_result = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

    // Check load
    if (audio_result != 0)
    {
        cout << "Failed to open audio: " << Mix_GetError() << endl;
    }

    // Load music
    music = Mix_LoadMUS("assets\\backgroundmusic.wav");
    if (!music)
    {
        cout << "Error loading music: " << Mix_GetError() << endl;
        return false;
    }

    // Load sound
    // sound = Mix_LoadWAV("scratch.wav");
    // if (!sound)
    // {
    //     cout << "Error loading sound: " << Mix_GetError() << endl;
    //     return false;
    // }

    // Play music forever
    Mix_PlayMusic(music, -1);
    return true;
}

// This is the function that handles all the events and inputs, from the player, also this is where we will display
// and move the blocks
bool main_game_loop()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            return false;
        }
        if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_RIGHT && dest.x < 300)
            {
                dest.x += 100;
            }
            if (e.key.keysym.sym == SDLK_LEFT && dest.x > 0)
            {
                dest.x -= 100;
            }
            if (e.key.keysym.sym == SDLK_SPACE)
            {
                if (Mix_PausedMusic())
                {
                    Mix_ResumeMusic();
                }
                else
                {
                    Mix_PauseMusic();
                }
            }
        }
    }

    // Clear the screen
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Handle falling block
    if (object_falling)
    {
        dest.y += 2; // Move block down
        if (dest.y >= 500)
        { // Stop when reaching the bottom
            object_falling = false;

            // Store block position in grid
            int row = dest.y / 100;
            int col = dest.x / 100;
            grid[row][col] = 'A' + (rand() % 26); // Assign random letter

            // Play block drop sound
            if (sound)
            {
                Mix_PlayChannel(-1, sound, 0);
            }
        }
    }
    else
    {
        // Generate a new block
        int random_letter = 'A' + (rand() % 26);
        string filename = "assets/" + to_string(random_letter) + ".png";
        SDL_Surface *block_image = IMG_Load(filename.c_str());
        if (block_image)
        {
            block_texture = SDL_CreateTextureFromSurface(renderer, block_image);
            SDL_FreeSurface(block_image);
        }

        BlockData newBlock;
        newBlock.texture = block_texture;
        newBlock.rect = dest;
        blocks.push_back(newBlock);

        object_falling = true;
    }

    // Draw blocks
    for (const auto &block : blocks)
    {
        SDL_RenderCopy(renderer, block.texture, NULL, &block.rect);
    }

    SDL_RenderPresent(renderer);
    return true;
}

// This frees the resources like window and renderer objects that we created earlier
void end()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    texture = NULL;
    window = NULL;
    renderer = NULL;
    object_falling = NULL;
    music = NULL;
    sound = NULL;
    IMG_Quit();
    // Mix_FreeChunk(sound);
    Mix_FreeMusic(music);

    Mix_Quit();
    SDL_Quit();
}
