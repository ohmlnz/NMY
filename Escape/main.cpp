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