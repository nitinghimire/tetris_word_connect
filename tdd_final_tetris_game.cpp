// build: g++ .\tdd_final_tetris_game.cpp -o tetris_final_game.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
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

#include <SDL2\SDL_ttf.h>
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

bool start_menu();
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

// the following array represents the occurance of the letters in the english words
// like the letter A represented as 1 and B represented by 2 and so on, until Z which
// is represented by 26
int random_numbers_representing_occurance_of_letter[] = {
    5, 20, 1, 8, 9, 15, 5, 5, 20, 18,
    3, 5, 21, 5, 16, 1, 12, 5, 9, 20,
    5, 26, 5, 20, 7, 15, 5, 19, 18, 14,
    14, 8, 12, 1, 5, 9, 4, 19, 5, 15,
    5, 5, 9, 20, 5, 1, 19, 18, 5, 15,
    9, 5, 24, 5, 4, 10, 5, 5, 12, 22,
    5, 1, 1, 21, 5, 9, 11, 5, 14, 5,
    14, 20, 4, 18, 5, 9, 17, 5, 5, 8,
    5, 25, 5, 23, 5, 15, 20, 5, 13, 9,
    20, 8, 1, 15, 20, 20, 12, 6, 19, 5,
    14, 7, 5, 18, 4, 1, 5, 14, 5, 13};

bool game_status = true;
// @some of the above objects may not be neccesary anymore

int main(int argc, char *argv[])
{
    // create a file stream object to read from a file
    ifstream fin;
    fin.open("4-letter-words.txt");

    string words[5272];
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

    if (!init())
    {
        return 1;
    }

    static SDL_Surface *block_image;

    static SDL_Rect dest;
    dest.w = 100;
    dest.h = 100;
    while (start_menu())
    {
        SDL_Delay(100);
    }

    static int score = 0;
    while (true)
    {
        Uint64 start = SDL_GetPerformanceCounter();

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // this is equivalent to blitting over the whole window to make sure the entire window is updated
        SDL_RenderClear(renderer);

        // this basically paints the background texture
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        // Render a score keeping system
        TTF_Font *font;
        // creates a font object fromt he font file, also can specify the font size in pixels
        font = TTF_OpenFont("fonts\\PixelifySans-VariableFont_wght.ttf", 16);
        if (!font)
        {
            cout << "Failed to load font: " << TTF_GetError() << endl;
        }
        SDL_Surface *text;
        SDL_Color color = {255, 255, 255};

        text = TTF_RenderText_Solid(font, ("SCORE:" + to_string(score)).c_str(), color);
        if (!text)
        {
            cout << "Failed to render text: " << TTF_GetError() << endl;
        }

        SDL_Texture *text_texture;

        text_texture = SDL_CreateTextureFromSurface(renderer, text);

        SDL_Rect dest_top_right = {300, 10, text->w, text->h};

        // SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, text_texture, NULL, &dest_top_right);
        SDL_DestroyTexture(text_texture);
        SDL_FreeSurface(text);

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
                    if (current_row != 5 && dest.x < 300 && grid[current_row][current_column + 1].char_rep == 0 && grid[current_row + 1][current_column + 1].char_rep == 0)
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
                if (e.key.keysym.sym == SDLK_RETURN && game_status == false)
                {
                    game_status = true;
                    SDL_Rect temp;
                    for (int i = 0; i < 6; i++)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            // set the value of the blockdata grid, character representation to 0
                            grid[i][j].char_rep = 0;
                            grid[i][j].rect = temp;
                            grid[i][j].texture = NULL;
                        }
                    }
                }
                {
                    if (e.key.keysym.sym == SDLK_LEFT)
                        // handle left movement
                        if (dest.x > 0 && current_row != 5 && grid[current_row][current_column - 1].char_rep == 0 && grid[current_row - 1][current_column - 1].char_rep == 0)
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

            static int count = 1;
            if (game_status == false)
            {
                SDL_Delay(200);
                cout
                    << "You lost! Play again.";
                SDL_Surface *game_over_image;
                SDL_Texture *gameover_texture;
                // here we will display the game over screen and continue the game loop,
                // because we still want to handle the input event to play again
                string filename = "assets/game_overr" + to_string(count) + ".jpeg";
                game_over_image = IMG_Load(filename.c_str());
                // Check load
                if (!game_over_image)
                {
                    cout << "Failed to load " << filename << ".png: " << IMG_GetError() << endl;
                }

                gameover_texture = SDL_CreateTextureFromSurface(renderer, game_over_image);

                SDL_FreeSurface(game_over_image);
                game_over_image = NULL;

                if (!gameover_texture)
                {
                    cout << "problem in texture loading" << endl;
                }
                SDL_RenderClear(renderer);

                // this basically paints the background texture
                SDL_RenderCopy(renderer, gameover_texture, NULL, NULL);
                SDL_RenderPresent(renderer);
                count = ((count + 1) % 5) + 1;
                continue;
            }
            if (grid[current_row][current_column].char_rep != 0)
            {
                game_status = false;
                continue;
            }
            block = (rand() % 99) + 1;
            block = random_numbers_representing_occurance_of_letter[block];
            // block = 1;
            grid[current_row][current_column]
                .char_rep = block;

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
            // sleep(1);
            for (int i = 0; i < 6; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    cout << char(grid[i][j].char_rep + 64) << "  ";
                    SDL_RenderCopy(renderer, grid[i][j].texture, NULL, &grid[i][j].rect);
                }
                cout << endl;
            }
            cout << endl
                 << endl
                 << endl;

            SDL_RenderPresent(renderer);
            SDL_Delay(500);
            continue;
        }

        if (block_falling == true)
        {
            if (current_row != 5 && grid[current_row + 1][current_column].char_rep == 0)
            {

                // increase the row of the block that is falling
                // also remove from the previous place in the multidimensional array
                // sleep(1);
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
                    temp.push_back(char(grid[current_row][0].char_rep + 64));
                    temp.push_back(char(grid[current_row][1].char_rep + 64));
                    temp.push_back(char(grid[current_row][2].char_rep + 64));
                    temp.push_back(char(grid[current_row][3].char_rep + 64));
                    cout << temp;
                    for (int i = 0; i < 5272; i++)
                    {
                        if (words[i] == temp)
                        {
                            // word is matched now need to delete row
                            cout << "row needs to be deleted" << endl;
                            score += 4;
                            for (int j = 3; j >= 0; j--)
                            {
                                for (int i = current_row; i >= 0; i--)
                                {
                                    if (i != 0 || grid[i - 1][j].char_rep != 0)
                                    {
                                        grid[i][j].char_rep = grid[i - 1][j].char_rep;
                                        // grid[i][j].rect = grid[i - 1][j].rect;
                                        grid[i][j].texture = grid[i - 1][j].texture;
                                        // SDL_Rect dummy;

                                        // grid[i - 1][j].rect = dummy;
                                        // grid[i - 1][j].char_rep = 0;
                                        // grid[i - 1][j].texture = NULL;
                                    }
                                    if (i == 0 || grid[i - 1][j].char_rep == 0)
                                    {
                                        BlockData temp;
                                        temp.char_rep = 0;
                                        temp.texture = NULL;
                                        grid[i][j] = temp;
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
                cout << char(grid[i][j].char_rep + 64) << "  ";
                SDL_RenderCopy(renderer, grid[i][j].texture, NULL, &grid[i][j].rect);
            }
            cout << endl;
        }
        SDL_RenderPresent(renderer);
        cout << endl
             << endl
             << endl;
        Uint64 end = SDL_GetPerformanceCounter();

        float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
        cout << "Current FPS: " << to_string(1.0f / elapsed) << endl;
        SDL_Delay(600);
    }

    end();
    // handle input
    return 0;
}
bool start_menu()
{
    // check events maybe
    // display some buttons and things
    // maybe show some animations
    // some interactive graphics
    // create some state to check whether the start_menu should be exited

    // SDL_Surface *frontpage_image;
    // SDL_Texture *frontpage_texture;
    // // here we will display the game over screen and continue the game loop,
    // // because we still want to handle the input event to play again
    // string filename = "assets/front_page.jpeg";
    // frontpage_image = IMG_Load(filename.c_str());
    // // Check load
    // if (!frontpage_image)
    // {
    //     cout << "Failed to load " << filename << ".png: " << IMG_GetError() << endl;
    // }

    // frontpage_texture = SDL_CreateTextureFromSurface(renderer, frontpage_image);

    // SDL_FreeSurface(frontpage_image);
    // frontpage_image = NULL;

    // if (!frontpage_texture)
    // {
    //     cout << "problem in texture loading" << endl;
    // }
    // SDL_RenderClear(renderer);

    // // this basically paints the background texture
    // SDL_RenderCopy(renderer, frontpage_texture, NULL, NULL);
    // SDL_RenderPresent(renderer);

    // instead of loading a static image as the start_menu
    // lets load fonts and try to create a dynamic kind of effect
    // lets see how we can do this using the sdl_tff library
    TTF_Font *font;
    // creates a font object fromt he font file, also can specify the font size in pixels
    font = TTF_OpenFont("fonts\\PixelifySans-VariableFont_wght.ttf", 100);
    if (!font)
    {
        cout << "Failed to load font: " << TTF_GetError() << endl;
    }
    // to actually render the fonts we need to create a surface or texture of some kind, and for that we have the rendertext function
    SDL_Surface *text;
    SDL_Surface *text2;
    SDL_Surface *text3;
    // Set color to random, this will cause a flickering effect on the font with different colors depending on the frame rate
    SDL_Color color = {(rand() % 256), (rand() % 256), (rand() % 256)};

    text = TTF_RenderText_Solid(font, " TETRIS :", color);
    if (!text)
    {
        cout << "Failed to render text: " << TTF_GetError() << endl;
    }
    color = {(rand() % 256), (rand() % 256), (rand() % 256)};
    text2 = TTF_RenderText_Solid(font, " WORD ", color);
    if (!text2)
    {
        cout << "Failed to render text: " << TTF_GetError() << endl;
    }
    color = {(rand() % 256), (rand() % 256), (rand() % 256)};
    text3 = TTF_RenderText_Solid(font, " GAME", color);
    if (!text3)
    {
        cout << "Failed to render text: " << TTF_GetError() << endl;
    }

    SDL_Texture *text_texture;

    text_texture = SDL_CreateTextureFromSurface(renderer, text);

    SDL_Rect dest = {0, 0, text->w, text->h};

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, text_texture, NULL, &dest);
    SDL_DestroyTexture(text_texture);
    text_texture = SDL_CreateTextureFromSurface(renderer, text2);
    dest.y += 200;
    SDL_RenderCopy(renderer, text_texture, NULL, &dest);
    SDL_DestroyTexture(text_texture);
    text_texture = SDL_CreateTextureFromSurface(renderer, text3);
    dest.y += 200;
    SDL_RenderCopy(renderer, text_texture, NULL, &dest);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text);
    SDL_FreeSurface(text2);
    SDL_FreeSurface(text3);

    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        switch (e.type)
        {
        case SDL_QUIT:
            game_status = false;
            return false;
        case SDL_MOUSEBUTTONDOWN:
            // if a state says to exit the start_menu and go to the main game loop then return false
            return false;
            break;
        }
    }
    return true;
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

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
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

    // text library initialization
    if (TTF_Init() < 0)
    {
        cout << "Error initializing SDL_ttf: " << TTF_GetError() << endl;
    }
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
