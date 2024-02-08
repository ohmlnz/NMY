#include "Scene.h"
#include "Game.h"

// use regular pointers if needed to increment nb of leaves to get perf boost
const int MAX_LEAVES = 100;
const int MIN_LEAVES = MAX_LEAVES / 2;

void Scene::loadLevel(const std::string& assetsPath, const std::string& levelPath)
{
	m_assets.initFont();

	std::ifstream assetsFile(assetsPath);
	
	// TODO: use struct
	std::string category, assetsTexture, assetPath;
	int boundingX, boundingY;

	// TODO: use an asset manager to handle all that crap
	if (!assetsFile)
	{
		std::cerr << "The assets file could not be opened at path: " << assetsPath << std::endl;
	}

	while (assetsFile >> category)
	{
		assetsFile >> assetsTexture >> assetPath >> boundingX >> boundingY;
		// TODO: add proper error handling - missing textures are not logged 
		SDL_Texture* texture = IMG_LoadTexture(m_gameEngine->currentRenderer(), assetPath.c_str());
		m_textures[assetsTexture] = TextureData{ texture, Vec2(boundingX, boundingY) };
		std::cout << "The following texture has been loaded: " << assetsTexture << std::endl;
	}

	// resets the entity manager
	m_entityManager = EntityManager();

	std::ifstream levelFile(levelPath);
	std::string type, levelTexture;
	int posX, posY;

	if (!levelFile)
	{
		std::cerr << "The level file could not be opened at path: " << levelPath << std::endl;
	}

	while (levelFile >> type)
	{
		levelFile >> levelTexture >> posX >> posY;

		SDL_Point size = m_entityManager.getTextureSize(m_textures[levelTexture].texture);
		Vec2 boundingBox = m_textures[levelTexture].boundingBox;

		posX *= m_window.BLOCK_SIZE;
		posY *= m_window.BLOCK_SIZE;

		// not sure if best way to manage initialization of structs, might wanna look into improving that
		if (type == "Player")
		{
			auto entity = std::make_shared<Playable>(type, levelTexture, Vec2(posX, posY), Vec2(32, 32), Vec2(32, 32));
			m_entityManager.addEntity(entity);
			m_player = entity;
		}
		else if (type == "Obstacle")
		{
			auto entity = std::make_shared<Obstacle>(type, levelTexture, Vec2(posX, posY), Vec2(float(size.x), float(size.y)), boundingBox);
			m_entityManager.addEntity(entity);
		}
		else
		{
			auto entity = std::make_shared<Decoration>(type, levelTexture, Vec2(posX, posY), Vec2(float(size.x), float(size.y)), boundingBox);
			m_entityManager.addEntity(entity);
		}
	}

	// TODO: not sure if this should be an entity?
	// auto entity = std::make_shared<Playable>("Blower", "", Vec2(m_player->m_position.x + m_player->m_half.x, m_player->m_position.y + m_player->m_half.y - m_player->m_size.y), Vec2(1, m_player->m_size.y * 2));
	// m_blower = m_entityManager.addEntity(entity);
	//generateLeaves();
	//m_entityManager.update();
	//m_score = m_entityManager.getEntities("Leaf").size();
}

void Scene::generateLeaves()
{
	int totalLeaves = m_utils.random(MIN_LEAVES, MAX_LEAVES);
	
	for (int i = 0; i < totalLeaves; i++)
	{
		int randomX = m_utils.random(0, m_window.SCREEN_WIDTH);
		int randomY = m_utils.random(m_window.BLOCK_SIZE, (m_window.SCREEN_HEIGHT - m_window.BLOCK_SIZE - 10));
		int leafIndex = m_utils.random(1, 4);
		std::string textureName = "TexLeaf" + std::to_string(leafIndex);
		auto entity = std::make_shared<Obstacle>("Leaf", textureName, Vec2(randomX, randomY), Vec2(32, 32), Vec2(32, 32));
		m_entityManager.addEntity(entity);
	}	
}

void Scene::restartGame()
{
	m_score = 0;
	m_player->m_position = { (m_window.SCREEN_WIDTH / 2) - m_player->m_half.x, (m_window.SCREEN_HEIGHT / 2) - m_player->m_half.y };
	generateLeaves();
	m_entityManager.update();
	m_score = m_entityManager.getEntities("Leaf").size();
	m_gameover = false;
}

void Scene::update()
{
	m_entityManager.update();
	handleTransform();
	handleCollision();
	handleScore();
}

void Scene::handleEvents(SDL_Event event)
{
	switch (event.type)
	{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)		
			{
				if (m_gameover)
				{
					case SDLK_y:
						restartGame();
						break;
					case SDLK_q:
						m_gameEngine->quit();
						break;
				}
				case SDLK_m:
					m_debugMode = !m_debugMode;
					break;
				case SDLK_RIGHT:
					m_player->m_right = true;
					m_player->m_direction = "right";
					break;
				case SDLK_LEFT:
					m_player->m_left = true;
					m_player->m_direction = "left";
					break;
				case SDLK_UP:
					m_player->m_up = true;
					break;
				case SDLK_DOWN:
					m_player->m_down = true;
					break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
				case SDLK_RIGHT:
					m_player->m_right = false;
					break;
				case SDLK_LEFT:
					m_player->m_left = false;
					break;
				case SDLK_UP:
					m_player->m_up = false;
					break;
				case SDLK_DOWN:
					m_player->m_down = false;
				break;
			}
		break;
	}
}

void Scene::handleTransform()
{
	Vec2& position = m_player->m_position;
	Vec2& velocity = m_player->m_velocity;
	m_player->m_previousPosition = position;

	if (m_player->m_right)
	{
		position.x += velocity.x;
	}
	else if (m_player->m_left)
	{
		position.x -= velocity.x;
	}

	if (m_player->m_up)
	{
		position.y -= velocity.y;
		// m_blower->m_position.y = m_player->m_position.y + m_player->m_half.y - m_player->m_size.y;
	}
	else if (m_player->m_down)
	{
		position.y += velocity.y;
		// m_blower->m_position.y = m_player->m_position.y + m_player->m_half.y - m_player->m_size.y;
	}	

	// TODO: we may want the blower to be the entity and the spread to be a property of that entity. something to think about.
	// TODO: use const values
	// TODO: to be refactored
	const int maxRange = 100;

	// if (m_player->m_direction == "right" || m_player->m_right)
	// {
	// 	m_blower->m_position.x += 1;
	// 
	// 	if (m_blower->m_position.x - (m_player->m_position.x + m_player->m_size.x) >= maxRange)
	// 	{
	// 		m_blower->m_position.x = m_player->m_position.x + m_player->m_half.x;
	// 	}
	// }
	// else if (m_player->m_direction == "left" || m_player->m_left)
	// {
	// 	m_blower->m_position.x -= 1;
	// 
	// 	if (m_player->m_position.x - m_blower->m_position.x >= maxRange)
	// 	{
	// 		m_blower->m_position.x = m_player->m_position.x + m_player->m_half.x;
	// 	}
	// }
}

void Scene::handleCollision()
{
	// TODO: filter by whats collidable only
	for (auto entity : m_entityManager.getEntities())
	{
		// TODO: add a collidable property or create a sub class
		if (entity->tag() == "Player" || entity->tag() == "Blower" || entity->tag() == "Decoration" || entity->m_boundingBox.x == 0 && entity->m_boundingBox.y == 0)
		{
			continue;
		}

		if (m_physics.isCollision(m_player, entity))
		{
			short int collisionDirection = m_physics.resolveOverlap(m_player, entity);
		}
	}

	for (auto entity : m_entityManager.getEntities("Leaf"))
	{
		if (m_physics.isCollision(m_blower, entity))
		{
			// TODO: add leaf physics to entity?
			entity->m_position.x += (m_player->m_direction == "right" ? 15 : -15);
			// randomize angle and use ease-in function so it looks kool.
			entity->m_angle += 1;

			// if out of bounds
			if (
				entity->m_position.x < 0 ||
				entity->m_position.x > m_window.SCREEN_WIDTH ||
				entity->m_position.y < 0 ||
				entity->m_position.y > m_window.SCREEN_HEIGHT
				)
			{
				entity->destroy();
				m_score--;
			}
		}
	}
}

void Scene::handleScore()
{
	if (m_score <= 0)
	{
		// m_gameover = true;
	}
}

void Scene::render()
{
	for (auto entity : m_entityManager.getEntities())
	{
		// TODO: get block width from const value
		SDL_Rect entityRect = { entity->m_position.x, entity->m_position.y, entity->m_size.x, entity->m_size.y };
		SDL_RenderCopyEx(
			m_gameEngine->currentRenderer(),
			m_textures[entity->texture()].texture,
			NULL,
			&entityRect,
			entity->m_angle,
			NULL,
			SDL_FLIP_NONE
		);


		if (m_debugMode)
		{
			m_debug.renderBoundingBoxes(m_gameEngine->currentRenderer(), entity);
		}

	}

	if (m_debugMode)
	{
		// m_debug.displayNumberOfLeaves(m_gameEngine->currentRenderer(), m_score);
		// m_debug.renderGridMode(m_gameEngine->currentRenderer());
		// 
	}

	if (m_gameover)
	{
		m_assets.displayText(
			"You win! Press Y to restart or Q to quit",
			m_window.SCREEN_WIDTH / 2 - 300,
			m_window.SCREEN_HEIGHT / 2,
			m_gameEngine->currentRenderer()
		);
	}
}