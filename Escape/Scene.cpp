#include "Scene.h"
#include "Game.h"

void Scene::loadLevel(const std::string& assetsPath, const std::string& levelPath)
{
	m_assets.initFont();

	std::ifstream assetsFile(assetsPath);
	std::string category;

	if (!assetsFile)
	{
		std::cerr << "The assets file could not be opened at path: " << assetsPath << "\n";
	}

	while (assetsFile >> category)
	{
		if (category == "Texture")
		{
			AssetData asset;
			assetsFile >> asset.texture >> asset.path >> asset.boundingX >> asset.boundingY;
			// TODO: add proper error handling - missing textures are not logged 
			SDL_Texture* texture = IMG_LoadTexture(m_gameEngine->currentRenderer(), asset.path.c_str());
			// TODO: use const window value when available
			// TODO: link bounding box attribute to animation data?
			m_textures[asset.texture] = TextureData{ texture, Vec2(asset.boundingX * 2, asset.boundingY * 2) };
			std::cout << "The following texture has been loaded: " << asset.texture << "\n";
		}

		if (category == "Animation")
		{
			AnimationData animation;
			assetsFile >> animation.name >> animation.textureName >> animation.frames >> animation.speed;
			SDL_Point size = m_entityManager.getTextureSize(m_textures[animation.textureName].texture);
			m_animations[animation.name] = Animation{
				animation.name,
				m_textures[animation.textureName].texture,
				Vec2(size.x, size.y),
				animation.frames,
				animation.speed
			};
			std::cout << "The following animation has been loaded: " << animation.name << "\n";
		}
	}

	// resets the entity manager
	m_entityManager = EntityManager();

	std::ifstream levelFile(levelPath);
	std::string type;

	if (!levelFile)
	{
		std::cerr << "The level file could not be opened at path: " << levelPath << "\n";
	}

	while (levelFile >> type)
	{
		LevelData level;
		levelFile >> level.texture >> level.posX >> level.posY;

		Vec2 boundingBox = m_textures["Tex" + level.texture].boundingBox;

		level.posX *= BLOCK_SIZE;
		level.posY *= BLOCK_SIZE;
		// TODO: use const window value when available
		Vec2 textureSize = { m_animations[level.texture].size().x * 2, m_animations[level.texture].size().y * 2 };

		if (type == "Player")
		{
			generateLeaves();
			std::shared_ptr entity = std::make_shared<Playable>(
				type,
				m_animations[level.texture],
				Vec2(level.posX, level.posY),
				Vec2(BLOCK_SIZE, BLOCK_SIZE),
				Vec2(BLOCK_SIZE, BLOCK_SIZE),
				Vec2(200, 200)
			);
			m_entityManager.addEntity(entity);
			m_player = entity;
		}
		else if (type == "Obstacle")
		{
			m_entityManager.addEntity(std::make_shared<Obstacle>(
				type,
				m_animations[level.texture],
				Vec2(level.posX, level.posY),
				textureSize,
				m_textures["Tex" + level.texture].boundingBox
			));
		}
		else
		{
			m_entityManager.addEntity(std::make_shared<Decoration>(
				type,
				m_animations[level.texture],
				Vec2(level.posX, level.posY),
				textureSize,
				m_textures["Tex" + level.texture].boundingBox
			));
		}
	}

	std::shared_ptr entity = std::make_shared<Playable>(
		"Blower",
		m_animations[""],
		Vec2(m_player->m_position.x + m_player->m_half.x, m_player->m_position.y + m_player->m_half.y - m_player->m_size.y),
		Vec2(1, m_player->m_size.y * 2),
		Vec2(1, m_player->m_size.y * 2),
		Vec2(500,500)
	);
	 m_entityManager.addEntity(entity);
	 m_blower = entity;
}

void Scene::generateLeaves()
{
	int totalLeaves = random(MIN_LEAVES, MAX_LEAVES);
	
	for (int i = 0; i < totalLeaves; i++)
	{
		int randomX = random(0, SCREEN_WIDTH);
		int randomY = random(BLOCK_SIZE, (SCREEN_HEIGHT - BLOCK_SIZE - 10));
		int leafIndex = random(1, 4);
		std::string textureName = "Leaf" + std::to_string(leafIndex);
		std::shared_ptr leaf = std::make_shared<Obstacle>(
			"Leaf",
			m_animations[textureName],
			Vec2(randomX, randomY),
			Vec2(BLOCK_SIZE, BLOCK_SIZE),
			Vec2(BLOCK_SIZE * 0.5, BLOCK_SIZE * 0.5)
		);
		m_entityManager.addEntity(leaf);
		m_entityManager.update();

		// remove leaves that are colliding with collidable entities
		for (std::shared_ptr obstacle : m_entityManager.getEntities("Obstacle"))
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
	m_gameover = false;
}

void Scene::update(float deltaTime)
{
	m_entityManager.update();
	handleState();
	handleAnimation();
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

					m_player->m_currentState = State::MOVING;
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

					m_player->m_currentState = State::MOVING;
					break;
				case SDLK_UP:
					m_player->m_up = true;
					m_player->m_currentState = State::MOVING;
					break;
				case SDLK_DOWN:
					m_player->m_down = true;
					m_player->m_currentState = State::MOVING;
					break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
				case SDLK_RIGHT:
					m_player->m_right = false;
					m_player->m_currentState = State::IDLING;
					break;
				case SDLK_LEFT:
					m_player->m_left = false;
					m_player->m_currentState = State::IDLING;
					break;
				case SDLK_UP:
					m_player->m_up = false;
					m_player->m_currentState = State::IDLING;
					break;
				case SDLK_DOWN:
					m_player->m_down = false;
					m_player->m_currentState = State::IDLING;
				break;
			}
			break;
	}
}

void Scene::handleState()
{
	switch (m_player->m_currentState)
	{
		case State::IDLING:
			if (m_player->m_animation.name() != "Stand")
			{
				m_player->m_animation = m_animations["Stand"];
			}
			break;

		case State::MOVING:
			if (m_player->m_animation.name() != "Run")
			{
				m_player->m_animation = m_animations["Run"];
			}
			break;
	}
}

void Scene::handleAnimation()
{
	for (std::shared_ptr entity : m_entityManager.getEntities())
	{
		entity->m_animation.update();
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

	if (m_player->m_direction == "right")
	{
		m_blower->m_position.x += m_blower->m_velocity.x * deltaTime;
	
		if (m_blower->m_position.x - (m_player->m_position.x + m_player->m_size.x) >= MAX_BLOWER_RANGE)
		{
			m_blower->m_position.x = (m_player->m_position.x + m_player->m_half.x);
		}
	}
	else if (m_player->m_direction == "left")
	{
		m_blower->m_position.x -= m_blower->m_velocity.x * deltaTime;
	
		if (m_player->m_position.x - m_blower->m_position.x >= MAX_BLOWER_RANGE)
		{
			m_blower->m_position.x = (m_player->m_position.x + m_player->m_half.x);
		}
	}

	// register leaves previous positions
	for (std::shared_ptr leaf : m_entityManager.getEntities("Leaf"))
	{
		Vec2& position = leaf->m_position;
		leaf->m_previousPosition = position;
	}
}

void Scene::handleCollision()
{
	for (std::shared_ptr entity : m_entityManager.getEntities())
	{
		if (entity->tag() == "Obstacle")
		{
			if (isCollision(m_player, entity))
			{
				resolveOverlap(m_player, entity);
			}

			for (std::shared_ptr leaf : m_entityManager.getEntities("Leaf"))
			{
				// TODO: refactor using a quadtree for better performance
				if (isCollision(leaf, entity))
				{
					// should bounce off
					int direction = resolveOverlap(leaf, entity);

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
	for (std::shared_ptr entity : m_entityManager.getEntities())
	{
		auto& sprite = entity->m_animation.sprite();
		SDL_Rect entityRect = { entity->m_position.x, entity->m_position.y, entity->m_size.x, entity->m_size.y };

		SDL_RenderCopyEx(
			m_gameEngine->currentRenderer(),
			entity->m_animation.texture(),
			SDL_RectEmpty(&sprite) ? NULL : &sprite,
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
