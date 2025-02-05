// build: g++ .\tdd_final_tetris_game.cpp -o tetris_final_game.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer
#include <iostream>
#include <bits/stdc++.h>
// Library effective with Windows
#include <windows.h>

// Library effective with Linux
#include <unistd.h>

// file handling
#include <fstream>

// Include the sdl library
#include <SDL2\SDL.h>
// include the image library and also the mixer library
#include <SDL2\SDL_image.h>
// include a mixer library to add music
#include <SDL2\SDL_mixer.h>

// import string library for filepath
#include <string>
using namespace std;

// now lets define some functions that seperate the functionality of the game

// Lets start by defining the initization part of the function called init()
bool init();

// The next part of the function lets call it main_game_loop()
// bool main_game_loop();
// @ might not be necessary

// lets create a load function that creates the blocks and

// Finally the final part of the program, where we close all the contexts and properly exit the program
void end();

struct BlockData
{
    SDL_Texture *texture;
    SDL_Rect rect; // To store position and size
    int char_rep;
};

// Now lets define some important contexts and pointers to various objects
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
SDL_Texture *block_texture;
SDL_Texture *main_texture;

Mix_Music *music;
Mix_Chunk *sound;

// @some of the above objects may not be neccesary anymore

int main(int argc, char *argv[])
{
    // create a file stream object to read from a file
    ifstream fin;
    fin.open("words.txt");

    string words[412];
    int index = 0;
    if (fin.good())
    {
        cout << "the words file was successfully opened!" << endl;
        do
        {
            // load the words into an array
            fin >> words[index];
            cout << words[index];
            ++index;
        } while (fin.good());
    }
    else
    {
        cout << "File was invalid " << endl;
    }

    // create an array
    // int grid[6][4] = {0};
    BlockData grid[6][4];

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            // set the value of the blockdata grid, character representation to 0
            grid[i][j].char_rep = 0;
        }
    }

    int block;
    // add the physics to array characters

    static bool block_falling = false;
    int current_column;
    int current_row;

    bool game_status = true;

    if (!init())
    {
        return 1;
    }

    static SDL_Surface *block_image;

    static SDL_Rect dest;
    dest.w = 100;
    dest.h = 100;

    while (true)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // this is equivalent to blitting over the whole window to make sure the entire window is updated
        SDL_RenderClear(renderer);

        // this basically paints the background texture
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        // handling the input events from keyboard
        SDL_Event e;
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
                    {
                        dest.x += 100;
                        SDL_Texture *temp = grid[current_row][current_column].texture;
                        grid[current_row][current_column].rect = dest;
                        grid[current_row][current_column].char_rep = 0;
                        grid[current_row][current_column].texture = NULL;
                        current_column++;
                        grid[current_row][current_column].char_rep = block;
                        grid[current_row][current_column].texture = temp;
                    }
                }

                if (e.key.keysym.sym == SDLK_LEFT)
                {
                    // handle left movement
                    if (dest.x > 0)
                    {
                        dest.x -= 100;
                        SDL_Texture *temp = grid[current_row][current_column].texture;
                        grid[current_row][current_column].rect = dest;
                        grid[current_row][current_column].char_rep = 0;
                        grid[current_row][current_column].texture = NULL;
                        current_column--;
                        grid[current_row][current_column].char_rep = block;
                        grid[current_row][current_column].texture = temp;
                    }
                }
                if (e.key.keysym.sym == SDLK_UP)
                {
                    // handle up movement
                }
                if (e.key.keysym.sym == SDLK_DOWN)
                {
                    // handle down movement
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
        // only create a new block if there is no block that is falling
        if (block_falling == false)
        {
            current_column = rand() % 4;
            dest.x = current_column * 100;
            current_row = 0;
            dest.y = current_row * 100;
            if (grid[current_row][current_column].char_rep != 0)
            {
                game_status = false;
                break;
            }
            // block = (rand() % 26) + 1;
            block = 1;
            grid[current_row][current_column].char_rep = block;

            // now we also have to create a block to be rendered:
            string filename = "assets/" + to_string(block) + ".jpeg";
            block_image = IMG_Load(filename.c_str());
            // Check load
            if (!block_image)
            {
                cout << "Failed to load " << filename << ".png: " << IMG_GetError() << endl;
            }

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
            newBlock.char_rep = block;
            grid[current_row][current_column] = newBlock;

            block_falling = true;
            sleep(1);
            for (int i = 0; i < 6; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    cout << char(grid[i][j].char_rep + 96) << "  ";
                    SDL_RenderCopy(renderer, grid[i][j].texture, NULL, &grid[i][j].rect);
                }
                cout << endl;
            }
            cout << endl
                 << endl
                 << endl;

            SDL_RenderPresent(renderer);
            continue;
        }

        if (block_falling == true)
        {
            if (current_row != 5 && grid[current_row + 1][current_column].char_rep == 0)
            {

                // increase the row of the block that is falling
                // also remove from the previous place in the multidimensional array
                sleep(1);
                SDL_Texture *temp = grid[current_row][current_column].texture;
                grid[current_row][current_column].char_rep = 0;
                grid[current_row][current_column].texture = NULL;
                current_row += 1;
                dest.y += 100;
                grid[current_row][current_column].char_rep = block;
                grid[current_row][current_column].texture = temp;
                grid[current_row][current_column].rect = dest;
                // print the array
            }
            else
            {
                // here check if the word is created, if it is created then, remove the row, and also set new row for each block above
                if (grid[current_row][0].char_rep != 0 &&
                    grid[current_row][1].char_rep != 0 &&
                    grid[current_row][2].char_rep != 0 &&
                    grid[current_row][3].char_rep != 0)
                {
                    string temp;
                    temp.push_back(char(grid[current_row][0].char_rep + 96));
                    temp.push_back(char(grid[current_row][1].char_rep + 96));
                    temp.push_back(char(grid[current_row][2].char_rep + 96));
                    temp.push_back(char(grid[current_row][3].char_rep + 96));
                    cout << temp;
                    for (int i = 0; i < 412; i++)
                    {
                        if (words[i] == temp)
                        {
                            // word is matched now need to delete row
                            cout << "row needs to be deleted" << endl;

                            for (int j = 3; j >= 0; j--)
                            {
                                for (int i = current_row; i >= 0; i--)
                                {
                                    if (i != 0 || grid[i - 1][j].char_rep != 0)
                                    {
                                        grid[i][j].char_rep = grid[i - 1][j].char_rep;
                                        grid[i][j].rect = grid[i - 1][j].rect;
                                        grid[i][j].texture = grid[i - 1][j].texture;
                                    }
                                    if (i == 0 || grid[i - 1][j].char_rep == 0)
                                    {
                                        BlockData temp;
                                        temp.char_rep = 0;
                                        grid[i][j] = temp;
                                        grid[i][j].texture = NULL;
                                    }
                                }
                            }

                            break;
                        }
                    }
                }
                current_row = NULL;
                current_column = NULL;
                block_falling = false;
            }
        }
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                cout << char(grid[i][j].char_rep + 96) << "  ";
                SDL_RenderCopy(renderer, grid[i][j].texture, NULL, &grid[i][j].rect);
            }
            cout << endl;
        }
        SDL_RenderPresent(renderer);
        cout << endl
             << endl
             << endl;
    }

    if (game_status == false)
    {
        cout << "You lost! Play again.";
    }

    end();
    // handle input
    return 0;
}

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

// // This frees the resources like window and renderer objects that we created earlier
void end()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    texture = NULL;
    window = NULL;
    renderer = NULL;
    // object_falling = NULL;
    music = NULL;
    sound = NULL;
    IMG_Quit();
    // Mix_FreeChunk(sound);
    Mix_FreeMusic(music);

    Mix_Quit();
    SDL_Quit();
}
