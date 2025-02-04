#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

const int ROWS = 6;
const int COLS = 4;
const int BLOCK_SIZE = 100;
char grid[ROWS][COLS] = {{'\0'}};

unordered_set<string> word_list;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
Mix_Music *music;

struct Block
{
    SDL_Texture *texture;
    SDL_Rect rect;
    char letter;
};
vector<Block> blocks;

bool object_falling = false;
Block current_block;

bool init();
void load_words();
bool main_game_loop();
void spawn_block();
void check_words();
void end();

int main(int argc, char **args)
{
    if (!init())
        return 1;
    load_words();
    spawn_block();

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
        return false;
    window = SDL_CreateWindow("Tetris Word Game", 500, 100, 400, 600, SDL_WINDOW_SHOWN);
    if (!window)
        return false;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
        return false;
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
    music = Mix_LoadMUS("assets/backgroundmusic.wav");
    if (music)
        Mix_PlayMusic(music, -1);
    return true;
}

void load_words()
{
    ifstream file("assets/words.txt");
    string word;
    while (file >> word)
        word_list.insert(word);
}

void spawn_block()
{
    int letter_index = rand() % 26 + 1;
    string filename = "assets/" + to_string(letter_index) + ".jpg";
    SDL_Surface *surface = IMG_Load(filename.c_str());
    if (!surface)
        return;
    current_block.texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    current_block.letter = (letter_index - 1);
    current_block.rect = {rand() % 4 * BLOCK_SIZE, -BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
    object_falling = true;
}

bool main_game_loop()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            return false;
        if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_RIGHT && current_block.rect.x < 300)
                current_block.rect.x += BLOCK_SIZE;
            if (e.key.keysym.sym == SDLK_LEFT && current_block.rect.x > 0)
                current_block.rect.x -= BLOCK_SIZE;
        }
    }

    SDL_RenderClear(renderer);
    for (auto &block : blocks)
    {
        SDL_RenderCopy(renderer, block.texture, NULL, &block.rect);
    }
    SDL_RenderCopy(renderer, current_block.texture, NULL, &current_block.rect);
    SDL_RenderPresent(renderer);

    int col = current_block.rect.x / BLOCK_SIZE;
    int row = (current_block.rect.y + BLOCK_SIZE) / BLOCK_SIZE;

    if (row < ROWS && grid[row][col] == '\0')
    {
        current_block.rect.y += 5;
    }
    else
    {
        blocks.push_back(current_block);
        grid[row - 1][col] = current_block.letter;
        object_falling = false;
        check_words();
        spawn_block();
    }
    return true;
}

void check_words()
{
    for (int r = 0; r < ROWS; ++r)
    {
        string word;
        for (int c = 0; c < COLS; ++c)
            word += grid[r][c];
        if (word_list.count(word))
        {
            for (int c = 0; c < COLS; ++c)
                grid[r][c] = '\0';
            blocks.erase(remove_if(blocks.begin(), blocks.end(), [r](Block &b)
                                   { return b.rect.y == r * BLOCK_SIZE; }),
                         blocks.end());
        }
    }
}

void end()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeMusic(music);
    Mix_Quit();
    SDL_Quit();
}
