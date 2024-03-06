#pragma once
#include "Scene.h"
#include "GameEngine.h"

const int MAX_INDEX = 2;

class SceneMenu : public Scene
{
private:
	int m_menuIndex = 0;
	const char* m_menuItems[3] = { "Play", "Settings", "Quit" };
public:
	SceneMenu(class GameEngine* gameEngine) : Scene(gameEngine) {}

	void process(SDL_Event event);
	void update(float deltaTime);
	void render();
};
