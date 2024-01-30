#include <SDL.h>
#include <stdio.h>

#include "Window.h"
#include "Game.h"

std::string assetsPath = "./assets.txt";
std::string levelPath = "./level.txt";

Window currentWindow;

int main(int argc, char* args[]) 
{
	Game game;
	game.init(currentWindow.SCREEN_WIDTH, currentWindow.SCREEN_HEIGHT, assetsPath, levelPath);
	game.run();

	return 0;
}

// TODO
// 1. add timer
// 2. improve blowing feature (should be progressive) and improve physics (should be fun, leaves should bounce around)
// 3. add assets (yard, house, then player, leaves and leaf blower)
// 4. add menu
// 5. add sounds