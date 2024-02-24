#pragma once
#include <SDL_image.h>
#include <string>
#include <fstream>
#include <iostream>

#include "Scene.h"
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
	float posX, posY;
};

class SceneMain : public Scene
{
private:
	std::map<std::string, TextureData>				m_textures;
	std::map<std::string, Animation>				m_animations;
	std::shared_ptr<Playable>						m_player;
	std::shared_ptr<Playable>						m_blower;
	bool											m_debugMode = false;
	int												m_score		= 0;
	bool											m_gameover  = false;
public:
	SceneMain(class Game* gameEngine) : Scene(gameEngine)
	{
		loadLevel();
	}

	void loadLevel();
	void generateLeaves();
	void restartGame();

	void process(SDL_Event event);
	void update(float deltaTime);
	void render();

	void handleState();
	void handleAnimation();
	void handleTransform(float deltaTime);
	void handleCollision();
	void handleScore();
};