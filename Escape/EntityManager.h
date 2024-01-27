#pragma once
#include "Entity.h"
#include <SDL.h>

#include <memory>
#include <vector>
#include <map>
#include <string>

class EntityManager
{
private:
	std::vector<std::shared_ptr<Entity>>							m_entities;
	std::map<std::string, std::vector<std::shared_ptr<Entity>>>		m_entitiesMap;
	std::vector<std::shared_ptr<Entity>>							m_entitiesToAdd;
	size_t															m_totalEntities = 0;
public:
	EntityManager();
	void update();
	void pushEntities();
	void removeEntities();
	SDL_Point getTextureSize(SDL_Texture* texture);
	std::shared_ptr<Entity> addEntity(std::shared_ptr<Entity> entity);
	std::vector<std::shared_ptr<Entity>>& getEntities();
	std::vector<std::shared_ptr<Entity>>& getEntities(const std::string& entityLabel);
};