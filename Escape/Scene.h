#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <SDL_image.h>
#include "EntityManager.h"
#include "Physics.h"
#include "Window.h"
#include "Debug.h"
#include "Assets.h"
#include "Utils.h"
#include "Math.h"

class Scene
{
private:
	class Game* m_gameEngine = nullptr;
	std::map<std::string, TextureData> m_textures;
	EntityManager m_entityManager;
	std::shared_ptr<Playable>						m_player;
	std::shared_ptr<Playable>						m_blower;
	Debug											m_debug;
	Assets											m_assets;
	bool											m_debugMode = false;
	int												m_score		= 0;
	bool											m_gameover  = false;
public:
	Scene(class Game* gameEngine) :
		m_gameEngine(gameEngine) {}
	void loadLevel(const std::string& assetsPath, const std::string& levelPath);
	void generateLeaves();
	void restartGame();

	void update();
	void render();

	void handleEvents(SDL_Event event);
	void handleTransform();
	void handleCollision();
	void handleScore();
};