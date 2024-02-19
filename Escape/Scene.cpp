#include "Scene.h"
#include "Game.h"

// use regular pointers if needed to increment nb of leaves to get perf boost
const int MAX_LEAVES = 10;
const int MIN_LEAVES = MAX_LEAVES / 2;

void Scene::loadLevel(const std::string& assetsPath, const std::string& levelPath)
{
	m_assets.initFont();

	std::ifstream assetsFile(assetsPath);
	
	// TODO: use struct
	std::string category, assetsTexture, assetPath;
	int boundingX, boundingY;
	
	std::string animationName, textureName;
	int frames, speed;

	// TODO: use an asset manager to handle all that crap
	if (!assetsFile)
	{
		std::cerr << "The assets file could not be opened at path: " << assetsPath << std::endl;
	}

	while (assetsFile >> category)
	{
		if (category == "Texture")
		{
			assetsFile >> assetsTexture >> assetPath >> boundingX >> boundingY;
			// TODO: add proper error handling - missing textures are not logged 
			SDL_Texture* texture = IMG_LoadTexture(m_gameEngine->currentRenderer(), assetPath.c_str());
			// TODO: use const window value when available
			m_textures[assetsTexture] = TextureData{ texture, Vec2(boundingX * 2, boundingY * 2) };
			std::cout << "The following texture has been loaded: " << assetsTexture << std::endl;
		}

		if (category == "Animation")
		{
			assetsFile >> animationName >> textureName >> frames >> speed;
			SDL_Point size = m_entityManager.getTextureSize(m_textures[textureName].texture);
			m_animations[animationName] = Animation{ animationName, m_textures[textureName].texture, Vec2(size.x * 2, size.y * 2), frames, speed };
			std::cout << "The following animation has been loaded: " << animationName << std::endl;
		}
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

		Vec2 boundingBox = m_textures["Tex" + levelTexture].boundingBox;

		posX *= BLOCK_SIZE;
		posY *= BLOCK_SIZE;

		// not sure if best way to manage initialization of structs, might wanna look into improving that
		if (type == "Player")
		{
			// generateLeaves();
			auto entity = std::make_shared<Playable>(
				type,
				m_animations[levelTexture],
				Vec2(posX, posY),
				Vec2(BLOCK_SIZE, BLOCK_SIZE), 
				Vec2(BLOCK_SIZE, BLOCK_SIZE),
				Vec2(3, 3)
			);
			m_entityManager.addEntity(entity);
			m_player = entity;
		}
		else if (type == "Obstacle")
		{
			auto entity = std::make_shared<Obstacle>(
				type,
				m_animations[levelTexture],
				Vec2(posX, posY),
				m_animations[levelTexture].size(),
				m_textures["Tex" + levelTexture].boundingBox
			);
			m_entityManager.addEntity(entity);
		}
		else
		{
			auto entity = std::make_shared<Decoration>(
				type,
				m_animations[levelTexture],
				Vec2(posX, posY),
				m_animations[levelTexture].size(),
				boundingBox
			);
			m_entityManager.addEntity(entity);
		}
	}

	// TODO: not sure if this should be an entity?
	auto entity = std::make_shared<Playable>(
		"Blower",
		m_animations[""],
		Vec2(m_player->m_position.x + m_player->m_half.x, m_player->m_position.y + m_player->m_half.y - m_player->m_size.y),
		Vec2(1, m_player->m_size.y * 2),
		Vec2(1, m_player->m_size.y * 2),
		Vec2(10,10)
	);
	 m_entityManager.addEntity(entity);
	 m_blower = entity;
	// m_entityManager.update();
	// m_score = m_entityManager.getEntities("Leaf").size();
}

void Scene::generateLeaves()
{
	int totalLeaves = random(MIN_LEAVES, MAX_LEAVES);
	
	for (int i = 0; i < totalLeaves; i++)
	{
		int randomX = random(0, SCREEN_WIDTH);
		int randomY = random(BLOCK_SIZE, (SCREEN_HEIGHT - BLOCK_SIZE - 10));
		int leafIndex = random(1, 4);
		std::string textureName = "TexLeaf" + std::to_string(leafIndex);
		auto leaf = std::make_shared<Obstacle>(
			"Leaf",
			m_animations[textureName],
			Vec2(randomX, randomY),
			Vec2(BLOCK_SIZE, BLOCK_SIZE),
			Vec2(BLOCK_SIZE * 0.5, BLOCK_SIZE * 0.5)
		);
		m_entityManager.addEntity(leaf);
		m_entityManager.update();

		// remove leaves that are colliding with collidable entities
		for (auto obstacle : m_entityManager.getEntities("Obstacle"))
		{
			if (isCollision(leaf, obstacle))
			{
				leaf->destroy();
			}
		}
	}	
}

void Scene::restartGame()
{
	m_score = 0;
	m_player->m_position = { (SCREEN_WIDTH / 2) - m_player->m_half.x, (SCREEN_HEIGHT / 2) - m_player->m_half.y };
	generateLeaves();
	m_entityManager.update();
	m_score = m_entityManager.getEntities("Leaf").size();
	m_gameover = false;
}

void Scene::update(float deltaTime)
{
	m_entityManager.update();
	handleTransform(deltaTime);
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
					m_player->m_flipped = false;

					// triggers only on first press
					if (event.key.repeat == 0)
					{
						m_blower->m_wasPositionReset = true;
					}
					break;
				case SDLK_LEFT:
					m_player->m_left = true;
					m_player->m_direction = "left";
					m_player->m_flipped = true;

					// triggers only on first press
					if (event.key.repeat == 0)
					{
						m_blower->m_wasPositionReset = true;
					}
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

void Scene::handleTransform(float deltaTime)
{
	Vec2& position = m_player->m_position;
	Vec2& velocity = m_player->m_velocity;
	m_player->m_previousPosition = position;

	if (m_player->m_right)
	{
		position.x += velocity.x * deltaTime;

		if (m_blower->m_wasPositionReset)
		{
			m_blower->m_position.x = (m_player->m_position.x + m_player->m_half.x);
			m_blower->m_wasPositionReset = false;
		}
	}
	else if (m_player->m_left)
	{
		position.x -= velocity.x * deltaTime;

		if (m_blower->m_wasPositionReset)
		{
			m_blower->m_position.x = (m_player->m_position.x + m_player->m_half.x);
			m_blower->m_wasPositionReset = false;
		}
	}

	if (m_player->m_up)
	{
		position.y -= velocity.y * deltaTime;
		m_blower->m_position.y = (m_player->m_position.y + m_player->m_half.y - m_player->m_size.y);
	}
	else if (m_player->m_down)
	{
		position.y += velocity.y * deltaTime;
		m_blower->m_position.y = (m_player->m_position.y + m_player->m_half.y - m_player->m_size.y);
	}	

	// TODO: we may want the blower to be the entity and the spread to be a property of that entity. something to think about.
	// TODO: use const values
	// TODO: to be refactored
	const int maxRange = 100;

	if (m_player->m_direction == "right")
	{
		m_blower->m_position.x += m_blower->m_velocity.x * deltaTime;
	
		if (m_blower->m_position.x - (m_player->m_position.x + m_player->m_size.x) >= maxRange)
		{
			m_blower->m_position.x = (m_player->m_position.x + m_player->m_half.x);
		}
	}
	else if (m_player->m_direction == "left")
	{
		m_blower->m_position.x -= m_blower->m_velocity.x * deltaTime;
	
		if (m_player->m_position.x - m_blower->m_position.x >= maxRange)
		{
			m_blower->m_position.x = (m_player->m_position.x + m_player->m_half.x);
		}
	}

	// register leaves previous positions
	for (auto leaf : m_entityManager.getEntities("Leaf"))
	{
		Vec2& position = leaf->m_position;
		leaf->m_previousPosition = position;
	}
}

void Scene::handleCollision()
{
	for (auto entity : m_entityManager.getEntities())	
	{
		if (entity->tag() == "Obstacle")
		{
			if (isCollision(m_player, entity))
			{
				resolveOverlap(m_player, entity);
			}

			for (auto leaf : m_entityManager.getEntities("Leaf"))
			{
				// TODO: refactor using a quadtree for better performance
				if (isCollision(leaf, entity))
				{
					// should bounce off
					short int direction = resolveOverlap(leaf, entity);

					// TODO: this should come from the blower, whose angle should change depending on the surface it hits
					leaf->m_position.y += random(1, 5);
				}
			}
		}

		if (entity->tag() == "Leaf")
		{
			if (isCollision(m_blower, entity))
			{
				// TODO: add better physics
				entity->m_position.x += (m_player->m_direction == "right" ? 15 : -15);
				// randomize angle and use ease-in function so it looks kool.
				entity->m_angle += 1;

				if (
					entity->m_position.x < 0 ||
					entity->m_position.x > SCREEN_WIDTH ||
					entity->m_position.y < 0 ||
					entity->m_position.y > SCREEN_HEIGHT
					)
				{
					entity->destroy();
					m_score--;
				}
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
		SDL_Rect entityRect = { entity->m_position.x, entity->m_position.y, entity->m_size.x, entity->m_size.y };
		SDL_RenderCopyEx(
			m_gameEngine->currentRenderer(),
			entity->m_animation.texture(),
			NULL,
			&entityRect,
			entity->m_angle,
			NULL,
			entity->m_flipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE
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
			SCREEN_WIDTH / 2 - 300,
			SCREEN_HEIGHT / 2,
			m_gameEngine->currentRenderer()
		);
	}
}