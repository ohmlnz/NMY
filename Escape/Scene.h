#pragma once
#include <SDL_image.h>
#include <string>
#include <fstream>
#include <iostream>

#include "EntityManager.h"
#include "Physics.h"
#include "Window.h"
#include "Debug.h"
#include "Assets.h"
#include "Utils.h"
#include "Math.h"
#include "Animation.h"

constexpr int MAX_LEAVES = 30;
constexpr int MIN_LEAVES = MAX_LEAVES / 2;
constexpr int MAX_BLOWER_RANGE = 100;

struct AssetData {
	std::string texture, path;
	int boundingX, boundingY;
};

struct AnimationData {
	std::string name, textureName;
	int frames, speed;
};

struct LevelData {
	std::string texture;
	int posX, posY;
};

class Scene
{
private:
	class Game*										m_gameEngine = nullptr;
	std::map<std::string, TextureData>				m_textures;
	std::map<std::string, Animation>				m_animations;
	EntityManager									m_entityManager;
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

	void update(float deltaTime);
	void render();

	void handleEvents(SDL_Event event);
	void handleState();
	void handleAnimation();
	void handleTransform(float deltaTime);
	void handleCollision();
	void handleScore();
};