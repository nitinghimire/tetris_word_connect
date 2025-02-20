// build command: g++ game.cpp -o game.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <bits/stdc++.h>
#include <string>
#include <vector>
using namespace std;

bool init();
bool main_game_loop();
void end();

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *background_texture;
SDL_Texture *block_texture;
Mix_Music *music;

struct BlockData
{
    SDL_Texture *texture;
    SDL_Rect rect;
};

vector<BlockData> blocks;
static bool object_falling = false;
const int ROWS = 6;
const int COLS = 4;
char grid[ROWS][COLS] = {{'\0'}};

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

bool init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        cout << "Error initializing SDL: " << SDL_GetError() << endl;
        return false;
    }
    window = SDL_CreateWindow("Tetris:Word Game", 500, 100, 400, 600, SDL_WINDOW_SHOWN);
    if (!window)
    {
        cout << "Error creating window: " << SDL_GetError() << endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        cout << "Error creating renderer: " << SDL_GetError() << endl;
        return false;
    }
    SDL_Surface *buffer = SDL_LoadBMP("assets/tetris-background.bmp");
    if (!buffer)
    {
        cout << "Error loading background image: " << SDL_GetError() << endl;
        return false;
    }
    background_texture = SDL_CreateTextureFromSurface(renderer, buffer);
    SDL_FreeSurface(buffer);
    if (!background_texture)
    {
        cout << "Error creating background texture: " << SDL_GetError() << endl;
        return false;
    }
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
    music = Mix_LoadMUS("assets/backgroundmusic.wav");
    if (!music)
    {
        cout << "Error loading music: " << Mix_GetError() << endl;
        return false;
    }
    Mix_PlayMusic(music, -1);
    return true;
}

bool main_game_loop()
{
    SDL_Event e;
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background_texture, NULL, NULL);
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
            return false;
        if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_LEFT)
            {
                if (!blocks.empty())
                {
                    BlockData &falling_block = blocks.back();
                    int new_x = falling_block.rect.x + (e.key.keysym.sym == SDLK_RIGHT ? 100 : -100);
                    if (new_x >= 0 && new_x < 400)
                        falling_block.rect.x = new_x;
                }
            }
        }
    }
    for (auto &block : blocks)
    {
        SDL_RenderCopy(renderer, block.texture, NULL, &block.rect);
    }
    SDL_RenderPresent(renderer);
    return true;
}

void end()
{
    SDL_DestroyTexture(background_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeMusic(music);
    Mix_Quit();
    SDL_Quit();
}
