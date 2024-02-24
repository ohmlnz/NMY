#pragma once
#include <SDL_image.h>

#include "EntityManager.h"
#include "Debug.h"
#include "Assets.h"

class Scene
{
protected:
	class Game*										m_gameEngine = nullptr;
	EntityManager									m_entityManager;
	Debug											m_debug;
	Assets											m_assets;

public:
	Scene(class Game* gameEngine) : m_gameEngine(gameEngine) {}

	virtual void process(SDL_Event event) = 0;
	virtual void update(float deltaTime) = 0;
	virtual void render() = 0;
};