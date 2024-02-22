#include <SDL.h>
#include <stdio.h>
#include <time.h>

#include "Game.h"
#include "Window.h"

const std::string assetsPath = "./assets.txt";
const std::string levelPath = "./level.txt";

int main(int argc, char* args[]) 
{
	srand(time(NULL));

	Game game;
	game.init(SCREEN_WIDTH, SCREEN_HEIGHT, assetsPath, levelPath);
	game.run();

	return 0;
}