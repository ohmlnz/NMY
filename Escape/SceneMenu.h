#pragma once
#include "Scene.h"
#include <iostream>

class SceneMenu : public Scene
{
public:
	SceneMenu(class Game* gameEngine) : Scene(gameEngine)
	{
		// init
		std::cout << "menu screen" << std::endl;
	}

	void process(SDL_Event event);
	void update(float deltaTime);
	void render();
};