# Word Tetris

Word Tetris is a word-building puzzle game inspired by the classic Tetris mechanics. Instead of forming solid rows, players must create valid four-letter words using falling letter blocks. The game is built using **C++** and **SDL2** for graphics, input handling, and audio.

## Features
- 🎮 **Real-time Gameplay**: Blocks fall dynamically, and players can move them left, right, and down.
- 🔤 **Word Formation**: Includes a built-in dictionary of four-letter words for validation.
- 🎶 **Audio Support**: Background music and sound effects enhance the gameplay experience.
- 📦 **Collision Handling**: Ensures blocks stack correctly without overlapping.
- 📊 **Score System**: Tracks player progress based on valid word formations.
- 🖥️ **Game Screens**: Includes a front screen (menu) and a back screen for better UI flow.

## Screenshots
| Stacking Example | Another Stacking Example |
|------------------|-------------------------|
| ![Stacking](https://github.com/user-attachments/assets/81cd8917-86f4-438d-a5c3-63607b0dd28b) | ![Stacking 2](https://github.com/user-attachments/assets/895048d7-eec4-44ce-8495-1e7489aed50b) |


## Installation & Running
### Prerequisites
- C++ Compiler (GCC/Clang/MSVC)
- SDL2 and its dependencies (`SDL2`, `SDL2_image`, `SDL2_mixer`)

### Build Instructions
1. Clone the repository:
   ```sh
   git clone https://github.com/yourusername/word-tetris.git
   cd word-tetris
   ```
## Compile the code:
 ```sh
   g++ -std=c++17 -o word_tetris main.cpp -lSDL2 -lSDL2_image -lSDL2_mixer
```
## Run the game:
```sh
./word_tetris
```

## Controls
- **Arrow Keys**: Move the falling blocks left, right, and down.  
- **Spacebar**: Pause/resume the background music.  

## Future Improvements
- ✅ Implement word validation for dynamic scoring.  
- ✅ Add more difficulty levels and dynamic grid scaling.  
- ✅ Improve UI elements and animations.  


## Credits
Developed by:

Nitin Ghimire
