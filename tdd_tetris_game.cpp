// build: g++ .\tdd_tetris_game.cpp -o tetris_game.exe
#include <iostream>
#include <bits/stdc++.h>
// Library effective with Windows
#include <windows.h>

// Library effective with Linux
#include <unistd.h>

// file handling
#include <fstream>

// // Include the sdl library
// #include <SDL2\SDL.h>
// // include the image library and also the mixer library
// #include <SDL2\SDL_image.h>
// // include a mixer library to add music
// #include <SDL2\SDL_mixer.h>
using namespace std;

struct BlockData
{
    // SDL_Texture *texture;
    // SDL_Rect rect; // To store position and size
    int char_rep;
};

// // Lets start by defining the initization part of the function called init()
// bool init();

// // Finally the final part of the program, where we close all the contexts and properly exit the program
// void end();

// // Now lets define some important contexts and pointers to various objects
// SDL_Window *window;
// SDL_Renderer *renderer;
// SDL_Texture *texture;
// SDL_Texture *block_texture;
// SDL_Texture *main_texture;

// Mix_Music *music;
// Mix_Chunk *sound;

int main(int argc, char const *argv[])
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

    // if (!init())
    // {
    //     return 1;
    // }

    while (true)
    {
        // only create a new block if there is no block that is falling
        if (block_falling == false)
        {
            current_column = rand() % 4;
            current_row = 0;
            if (grid[current_row][current_column].char_rep != 0)
            {
                game_status = false;
                break;
            }
            // block = (rand() % 26) + 1;
            block = 1;
            grid[current_row][current_column].char_rep = block;
            block_falling = true;
            sleep(1);
            for (int i = 0; i < 6; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    cout << char(grid[i][j].char_rep + 96) << "  ";
                }
                cout << endl;
            }
            cout << endl
                 << endl
                 << endl;
            continue;
        }

        if (block_falling == true)
        {
            if (current_row != 5 && grid[current_row + 1][current_column].char_rep == 0)
            {

                // increase the row of the block that is falling
                // also remove from the previous place in the multidimensional array
                sleep(0.5);
                grid[current_row][current_column].char_rep = 0;
                current_row++;
                grid[current_row][current_column].char_rep = block;
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
                                    if (i != 0)
                                        grid[i][j].char_rep = grid[i - 1][j].char_rep;
                                    if (i == 0)
                                        grid[i][j].char_rep = 0;
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
            }
            cout << endl;
        }
        cout << endl
             << endl
             << endl;
    }

    if (game_status == false)
    {
        cout << "You lost! Play again.";
    }

    // end();
    // handle input
    return 0;
}

// bool init()
// {

//     if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
//     {
//         cout << "Error initializing SDL: " << SDL_GetError() << endl;
//         system("pause");
//         return false;
//     }

//     window = SDL_CreateWindow("Tetris:Word Game", 500, 100, 400, 600, SDL_WINDOW_SHOWN);
//     if (!window)
//     {
//         cout << "Error creating window: " << SDL_GetError() << endl;
//         system("pause");
//         return false;
//     }

//     renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//     if (!renderer)
//     {
//         cout << "Error creating renderer: " << SDL_GetError() << endl;
//         return false;
//     }

//     // Load bitmap into surface
//     SDL_Surface *buffer = SDL_LoadBMP("assets\\tetris-background.bmp");
//     if (!buffer)
//     {
//         cout << "Error loading image tetris-background.bmp: " << SDL_GetError() << endl;
//         return false;
//     }

//     // Create texture
//     texture = SDL_CreateTextureFromSurface(renderer, buffer);
//     // Free surface as it's no longer needed
//     SDL_FreeSurface(buffer);
//     buffer = NULL;
//     if (!texture)
//     {
//         cout << "Error creating texture: " << SDL_GetError() << endl;
//         return false;
//     }

//     int result = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

//     // Check load
//     if (result == 0)
//     {
//         cout << "Failed to initialize SDL_image: " << IMG_GetError() << endl;
//     }

//     cout << "SDL_image initialized successfully!" << endl;

//     main_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 400, 600);
//     SDL_RenderClear(renderer);

//     int audio_result = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

//     // Check load
//     if (audio_result != 0)
//     {
//         cout << "Failed to open audio: " << Mix_GetError() << endl;
//     }

//     // Load music
//     music = Mix_LoadMUS("assets\\backgroundmusic.wav");
//     if (!music)
//     {
//         cout << "Error loading music: " << Mix_GetError() << endl;
//         return false;
//     }

//     // Load sound
//     // sound = Mix_LoadWAV("scratch.wav");
//     // if (!sound)
//     // {
//     //     cout << "Error loading sound: " << Mix_GetError() << endl;
//     //     return false;
//     // }

//     // Play music forever
//     Mix_PlayMusic(music, -1);
//     return true;
// }

// // This frees the resources like window and renderer objects that we created earlier
// void end()
// {
//     SDL_DestroyTexture(texture);
//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);
//     texture = NULL;
//     window = NULL;
//     renderer = NULL;
//     // object_falling = NULL;
//     music = NULL;
//     sound = NULL;
//     IMG_Quit();
//     // Mix_FreeChunk(sound);
//     Mix_FreeMusic(music);

//     Mix_Quit();
//     SDL_Quit();
// }
