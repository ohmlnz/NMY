#include <SDL.h>
#include <stdio.h>
#include "Window.h"
#include "Game.h"
#include <time.h> 

// TODO: get config from file
std::string assetsPath = "./assets.txt";
std::string levelPath = "./level.txt";

int main(int argc, char* args[]) 
{
	srand(time(NULL));

	Game game;
	game.init(SCREEN_WIDTH, SCREEN_HEIGHT, assetsPath, levelPath);
	game.run();
	return 0;
}

// TODO
// 1. add timer
// 2. improve blowing feature (should be progressive) and improve physics (should be fun, leaves should bounce around)
// 3. add assets (yard, house, then player, leaves and leaf blower)
// 4. add menu
// 5. add sounds