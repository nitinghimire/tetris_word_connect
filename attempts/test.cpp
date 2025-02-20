#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string>

using namespace std;

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 600;
const int BLOCK_SIZE = 100;
const int GRID_ROWS = 6;
const int GRID_COLS = 4;

struct Block
{
    SDL_Texture *texture;
    SDL_Rect rect;
    char letter;
};

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Texture *backgroundTexture = nullptr;
Mix_Music *music = nullptr;
vector<Block> blocks;
char grid[GRID_ROWS][GRID_COLS] = {{0}};

bool init();
bool loadMedia();
void close();
void renderGrid();
bool handleEvents();
void dropBlock();

int main()
{
    if (!init() || !loadMedia())
        return 1;
    bool running = true;

    while (running)
    {
        running = handleEvents();
        dropBlock();
        renderGrid();
        SDL_Delay(100);
    }

    close();
    return 0;
}

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
        return false;
    window = SDL_CreateWindow("Word Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
        return false;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
        return false;
    if (IMG_Init(IMG_INIT_PNG) == 0)
        return false;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        return false;
    return true;
}

bool loadMedia()
{
    SDL_Surface *tempSurface = IMG_Load("assets/tetris-background.png");
    if (!tempSurface)
        return false;
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    music = Mix_LoadMUS("assets/backgroundmusic.wav");
    if (music)
        Mix_PlayMusic(music, -1);
    return true;
}