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
#include "Utils.h"
#include "Math.h"
#include "Animation.h"
#include "Math.h"

const int MAX_LEAVES = 50;
const int MIN_LEAVES = MAX_LEAVES / 2;
const int MAX_BLOWER_RANGE = 100;
const std::string ASSETS_PATH = "./assets.txt";
const std::string LEVEL_PATH = "./level.txt";

struct TextureData
{
	SDL_Texture* texture;
	Vec2 boundingBox;
};

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
	bool											m_debugMode = true;
	int												m_score		= 0;
	int												m_leaves;
	bool											m_gameover  = false;
	float											m_fps;
public:
	SceneMain(class GameEngine* gameEngine) : Scene(gameEngine)
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
	void handleCollision(float deltaTime);
	void handleScore();
};