# Not In My Yard!!

## Overview
"Not In My Yard!!" is a 2D game developed using SDL (Simple DirectMedia Layer) and C++. The game involves various game elements such as a player, obstacles, decorations, and animations. The goal is to blow all the leaves that randomly appear on the screen as quickly as possible. The game engine handles the initialization, running, and quitting of the game, while different scenes manage the game states, such as the main menu and the main game scene.

![nmy-gameplay-opti](https://github.com/user-attachments/assets/83eb6b64-4335-4c74-87ed-0b367b42b369)

## Project Structure
The project is organized into several key components:

- **Game Engine**: Manages the game loop, rendering, and scene transitions. Implemented in [`GameEngine.cpp`](GameEngine.cpp) and [`GameEngine.h`](GameEngine.h).
- **Scenes**: Different game states, such as the main menu and the main game scene. Implemented in [`SceneMenu.cpp`](SceneMenu.cpp), [`SceneMenu.h`](SceneMenu.h), [`SceneMain.cpp`](SceneMain.cpp), and [`SceneMain.h`](SceneMain.h).
- **Entities**: Game objects like the player, obstacles, and decorations. Defined in [`Entity.h`](Entity.h) and managed by [`EntityManager.cpp`](EntityManager.cpp) and [`EntityManager.h`](EntityManager.h).
- **Physics**: Handles collision detection and resolution. Implemented in [`Physics.cpp`](Physics.cpp) and [`Physics.h`](Physics.h).
- **Animations**: Manages sprite animations for game entities. Implemented in [`Animation.cpp`](Animation.cpp) and [`Animation.h`](Animation.h).
- **Utilities**: Provides helper functions like random number generation. Implemented in [`Utils.cpp`](Utils.cpp) and [`Utils.h`](Utils.h).
- **Debug**: Provides tools for debugging the game, such as logging and visual debug information. Implemented in [`Debug.cpp`](Debug.cpp) and [`Debug.h`](Debug.h).

## Assets
The game uses various assets such as textures and fonts, which are organized in the `assets` directory. The assets are listed and loaded directly from [`assets.txt`](assets.txt).

## How to Build and Run
1. Ensure you have the following libraries: `SDL`, `SDL_image`, and `SDL_ttf`.
2. Compile the project using your preferred C++ compiler.
3. Run the executable to start the game.

## Controls
- **Arrow Keys**: Move the player.
- **M**: Toggle debug mode.
- **Y**: Restart the game (when game over).
- **Q**: Quit the game.

## License
This project is licensed under the MIT License.
