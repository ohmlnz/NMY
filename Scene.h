#pragma once
#include <SDL_image.h>

#include "EntityManager.h"
#include "Debug.h"
#include "Text.h"

class Scene
{
protected:
	class GameEngine*								m_gameEngine = nullptr;
	EntityManager									m_entityManager;
	Debug											m_debug;
	Text											m_text;

public:
	Scene(class GameEngine* gameEngine) : m_gameEngine(gameEngine) {}

	virtual void process(SDL_Event event) = 0;
	virtual void update(float deltaTime) = 0;
	virtual void render() = 0;
};