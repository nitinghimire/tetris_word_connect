// build: g++ .\tdd_tetris_array_test.cpp -o arraytest.exe
#include <iostream>
#include <bits/stdc++.h>
// Library effective with Windows
#include <windows.h>

// Library effective with Linux
#include <unistd.h>

// file handling
#include <fstream>
using namespace std;

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
    int grid[6][4] = {0};

    int block;
    // add the physics to array characters

    static bool block_falling = false;
    int current_column;
    int current_row;

    bool game_status = true;
    while (true)
    {
        // only create a new block if there is no block that is falling
        if (block_falling == false)
        {
            current_column = rand() % 4;
            current_row = 0;
            if (grid[current_row][current_column] != 0)
            {
                game_status = false;
                break;
            }
            // block = (rand() % 26) + 1;
            block = 1;
            grid[current_row][current_column] = block;
            block_falling = true;
            sleep(1);
            for (int i = 0; i < 6; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    cout << char(grid[i][j] + 96) << "  ";
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
            if (current_row != 5 && grid[current_row + 1][current_column] == 0)
            {

                // increase the row of the block that is falling
                // also remove from the previous place in the multidimensional array
                sleep(0.5);
                grid[current_row][current_column] = 0;
                current_row++;
                grid[current_row][current_column] = block;
                // print the array
            }
            else
            {
                // here check if the word is created, if it is created then, remove the row, and also set new row for each block above
                if (grid[current_row][0] != 0 &&
                    grid[current_row][1] != 0 &&
                    grid[current_row][2] != 0 &&
                    grid[current_row][3] != 0)
                {
                    string temp;
                    temp.push_back(char(grid[current_row][0] + 96));
                    temp.push_back(char(grid[current_row][1] + 96));
                    temp.push_back(char(grid[current_row][2] + 96));
                    temp.push_back(char(grid[current_row][3] + 96));
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
                                        grid[i][j] = grid[i - 1][j];
                                    if (i == 0)
                                        grid[i][j] = 0;
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
                cout << char(grid[i][j] + 96) << "  ";
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
    // handle input
    return 0;
}
