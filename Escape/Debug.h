#pragma once
#include <SDL.h>
#include <memory>

#include "Entity.h"
#include "Window.h"
#include "EntityManager.h"
#include "Assets.h"

// TODO: complement with imGUI?  
class Debug
{
private:
	Window m_window;
	EntityManager m_entityManager;
	Assets m_assets;

public:
	void displayNumberOfLeaves(SDL_Renderer* renderer, int& score)
	{
		std::string label = "Nb of leaves: " + std::to_string(score);
		// m_assets.displayText(label.c_str(), 2 * m_window.BLOCK_SIZE, 2 * m_window.BLOCK_SIZE, renderer);
	}

	void renderGridMode(SDL_Renderer* renderer)
	{
		// TODO: all the values below should be dynamic
		const int BLOCK_SIZE = 30;
		for (int i = 0; i < 960 / BLOCK_SIZE; i++)
		{
			SDL_RenderDrawLine(
				renderer,
				i * BLOCK_SIZE,
				0,
				i * BLOCK_SIZE,
				960
			);

			for (int j = 0; j < 540 / BLOCK_SIZE; j++)
			{
				SDL_RenderDrawLine(
					renderer,
					0,
					j * BLOCK_SIZE,
					960,
					j * BLOCK_SIZE
				);

				std::string label = "(" + std::to_string(i) + "," + std::to_string(j) + ")";
				// m_assets.displayText(label.c_str(), i * BLOCK_SIZE, j * BLOCK_SIZE, renderer);
			}
		}
	}

	void renderBoundingBoxes(SDL_Renderer* renderer, const std::shared_ptr<Entity>& entity)
	{
		if (entity->m_boundingBox.x == 0 && entity->m_boundingBox.y == 0)
		{
			return;
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		Vec2 entityCenterPoint = { entity->m_position.x + entity->m_half.x, entity->m_position.y + entity->m_half.y };
		Vec2 boundingBoxPosition = { entityCenterPoint.x - entity->m_boundingBox.x / 2, entityCenterPoint.y - entity->m_boundingBox.y / 2 };
		SDL_Rect rect = { boundingBoxPosition.x, boundingBoxPosition.y, entity->m_boundingBox.x, entity->m_boundingBox.y };
		SDL_RenderDrawRect(renderer, &rect);
	}
}; 