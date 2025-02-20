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
    static SDL_Surface *block_image;
    static int image_loading;

    SDL_Event e;
    static SDL_Rect dest;
    dest.w = 100;
    dest.h = 100;

    // this is equivalent to blitting over the whole window to make sure the entire window is updated
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // render the other blocks here as well

    while (SDL_PollEvent(&e) != 0)
    {
        switch (e.type)
        {
        case SDL_QUIT:
            return false;
            break;
        case SDL_KEYDOWN:
            if (e.key.keysym.sym == SDLK_RIGHT)
            {

                // handle right movement
                if (dest.x < 300)
                    dest.x += 100;
            }

            if (e.key.keysym.sym == SDLK_LEFT)
            {
                // handle left movement
                if (dest.x > 0)
                    dest.x -= 100;
            }
            if (e.key.keysym.sym == SDLK_UP)
            {
                // handle up movement
            }
            if (e.key.keysym.sym == SDLK_DOWN)
            {
                if (dest.y <= 300)
                    dest.y += 50;
            }

            if (e.key.keysym.sym == SDLK_SPACE)
            {
                if (Mix_PausedMusic() == 1)
                {
                    Mix_ResumeMusic();
                }
                else
                {
                    Mix_PauseMusic();
                }
            }
            else if (e.key.keysym.sym == SDLK_ESCAPE)
            {
                Mix_HaltMusic();
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            // Play sound once on the first available channel
            Mix_PlayChannel(-1, sound, 0);
            break;

        default:
            // handle down movement
            break;
        }
    }

    if (object_falling)
    {
        int column = dest.x / 100 - 1;
        int row;
        if (dest.y < 100)
            row = 0;
        else if (dest.y < 200)
            row = 1;
        else if (dest.y < 300)
            row = 2;
        else if (dest.y < 400)
            row = 3;
        else if (dest.y < 500)
            row = 4;
        else
            row = 5;

        if (grid[row][column] == '\0' && row != 5)
        {
            // column = column - 1;
            dest.y += 2;
        }
        if (grid[row][column] == '\0' && row == 5)
        {
            // column = 0;
            grid[row][column] = image_loading;
            // dest.y = -100;
            // int value = ((rand() % 300) + 1);
            // if (value > 100)
            //     dest.x = 0;
            // else if (value > 200)
            //     dest.x = 100;
            // else if (value > 300)
            //     dest.x = 200;
            // else
            //     dest.x = 300;
            object_falling = false;
        }

        // if (dest.y < (column * 100 + 100))
        //     dest.y += 2;
        // else
        // {
        //     grid[row][column] = image_loading;
        //     dest.y = -100;
        //     int value = ((rand() % 300) + 1);
        //     if (value > 100)
        //         dest.x = 0;
        //     else if (value > 200)
        //         dest.x = 100;
        //     else if (value > 300)
        //         dest.x = 200;
        //     else
        //         dest.x = 300;
        //     object_falling = false;
        // }
    }
    else
    {
        for (const auto &block : blocks)
        {
            SDL_RenderCopy(renderer, block.texture, NULL, &block.rect); // Use the rect from BlockData
        }

        // SDL_DestroyTexture(block_texture);
        // // generate file
        // const char *filename = ("assets/" + to_string(((rand() % 26) + 1)) + ".jpeg").c_str();
        // // create a new falling object
        // block_image = IMG_Load(filename);

        block_texture = SDL_CreateTextureFromSurface(renderer, block_image);

        SDL_FreeSurface(block_image);
        block_image = NULL;

        if (!block_texture)
        {
            cout << "problem in texture loading" << endl;
        }

        BlockData newBlock;
        newBlock.texture = block_texture;
        newBlock.rect = dest;
        blocks.push_back(newBlock);
        // newBlock = NULL;

        int random_letter = ((rand() % 26) + 1);
        string filename = "assets/" + to_string(random_letter) + ".jpeg";
        image_loading = random_letter;
        block_image = IMG_Load(filename.c_str());
        // Check load
        if (!block_image)
        {
            cout << "Failed to load " << filename << ".png: " << IMG_GetError() << endl;
        }

        SDL_RenderPresent(renderer);
        dest.y = -20;
        dest.x = int((rand() % 400) / 100);
        object_falling = true;
        SDL_RenderCopy(renderer, block_texture, NULL, &dest);
        SDL_RenderClear(renderer);
    }

    // cout << (rand() % 26) + 1 << endl;
    SDL_RenderCopy(renderer, block_texture, NULL, &dest);
    SDL_RenderPresent(renderer);
    // SDL_SetRenderTarget(renderer, NULL);

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
