#include "SceneMain.h"
#include "GameEngine.h"

void SceneMain::loadLevel()
{
	std::ifstream assetsFile(ASSETS_PATH);
	std::string category;

	if (!assetsFile)
	{
		std::cerr << "The assets file could not be opened at path: " << ASSETS_PATH << "\n";
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

	std::ifstream levelFile(LEVEL_PATH);
	std::string type;

	if (!levelFile)
	{
		std::cerr << "The level file could not be opened at path: " << LEVEL_PATH << "\n";
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
				Vec2(0.8 * BLOCK_SIZE, 0.8 * BLOCK_SIZE),
				Vec2(300, 300)
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
		m_animations["Wind"],
		// TODO: position + half should bring to center not far end, this is related to the way entites are scaled
		Vec2(m_player->m_position.x + m_player->m_half.x, m_player->m_position.y),
		Vec2(BLOCK_SIZE, BLOCK_SIZE),
		Vec2(1, m_player->m_size.y * 2),
		Vec2(400,400)
	);
	 m_entityManager.addEntity(entity);
	 m_blower = entity;
}

void SceneMain::generateLeaves()
{
	int totalLeaves = random(MIN_LEAVES, MAX_LEAVES);
	m_leaves = totalLeaves;

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
				m_leaves--;
			}
		}
	}	
}

void SceneMain::restartGame()
{
	m_score = 0;
	m_player->m_position = { (SCREEN_WIDTH / 2) - m_player->m_half.x, (SCREEN_HEIGHT / 2) - m_player->m_half.y };
	generateLeaves();
	m_entityManager.update();
	m_gameover = false;
}

void SceneMain::update(float deltaTime)
{
	m_fps = deltaTime;
	m_entityManager.update();
	handleState();
	handleAnimation();
	handleTransform(deltaTime);
	handleCollision(deltaTime);
	handleScore();
}

void SceneMain::process(SDL_Event event)
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

void SceneMain::handleState()
{
	if (!m_player->m_right && !m_player->m_left && !m_player->m_up && !m_player->m_down)
	{
		m_player->m_currentState = State::IDLING;
	}

	if (m_player->m_right || m_player->m_left || m_player->m_up || m_player->m_down)
	{
		m_player->m_currentState = State::MOVING;
	}

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

void SceneMain::handleAnimation()
{
	for (std::shared_ptr entity : m_entityManager.getEntities())
	{
		entity->m_animation.update();
	}
}

void SceneMain::handleTransform(float deltaTime)
{
	Vec2& playerPosition = m_player->m_position;
	Vec2& playerVelocity = m_player->m_velocity;
	Vec2& playerHalf = m_player->m_half;
	m_player->m_previousPosition = playerPosition;

	Vec2& blowerPosition = m_blower->m_position;
	Vec2& blowerVelocity = m_blower->m_velocity;
	bool& blowerPositionReset = m_blower->m_wasPositionReset;

	if (m_player->m_right)
	{
		playerPosition.x += playerVelocity.x * deltaTime;

		if (blowerPositionReset)
		{
			blowerPosition.x = (playerPosition.x + playerHalf.x);
			blowerPositionReset = false;
		}
	}
	else if (m_player->m_left)
	{
		playerPosition.x -= playerVelocity.x * deltaTime;

		if (blowerPositionReset)
		{
			blowerPosition.x = (playerPosition.x - playerHalf.x);
			blowerPositionReset = false;
		}
	}

	if (m_player->m_up)
	{
		playerPosition.y -= playerVelocity.y * deltaTime;
		blowerPosition.y = playerPosition.y;
	}
	else if (m_player->m_down)
	{
		playerPosition.y += playerVelocity.y * deltaTime;
		blowerPosition.y = playerPosition.y;
	}	

	if (m_player->m_direction == "right")
	{
		blowerPosition.x += blowerVelocity.x * deltaTime;
	
		if (blowerPosition.x - (playerPosition.x + playerHalf.x) >= MAX_BLOWER_RANGE)
		{
			blowerPosition.x = playerPosition.x + playerHalf.x;
		}
	}
	else if (m_player->m_direction == "left")
	{
		blowerPosition.x -= blowerVelocity.x * deltaTime;
	
		if ((playerPosition.x - playerHalf.x) - blowerPosition.x >= MAX_BLOWER_RANGE)
		{
			blowerPosition.x = playerPosition.x - playerHalf.x;
		}
	}

	// register leaves previous positions
	for (std::shared_ptr leaf : m_entityManager.getEntities("Leaf"))
	{
		Vec2& position = leaf->m_position;
		leaf->m_previousPosition = position;
	}
}

void SceneMain::handleCollision(float deltaTime)
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
					m_leaves--;
				}
			}
		}
	}

}

void SceneMain::handleScore()
{
	if (m_score <= 0)
	{
		// m_gameover = true;
	}
}

void SceneMain::render()
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

	// debug info
	if (m_debugMode)
	{
		std::string displayLeaves = "Number of leaves: " + std::to_string(m_leaves);
		m_text.displayText(displayLeaves.c_str(), 50, 50, m_gameEngine->currentRenderer());

		std::string displayFPS = "Frames per seconds: " + std::to_string(static_cast <int>(1000 / (m_fps * 1000)));
		m_text.displayText(displayFPS.c_str(), 50, 80, m_gameEngine->currentRenderer());

		// m_debug.displayNumberOfLeaves(m_gameEngine->currentRenderer(), m_score);
		// m_debug.renderGridMode(m_gameEngine->currentRenderer()); 
	}

	if (m_gameover)
	{
		// m_text->displayText(
		// 	"You win! Press Y to restart or Q to quit",
		// 	SCREEN_WIDTH / 2 - 300,
		// 	SCREEN_HEIGHT / 2,
		// 	m_gameEngine->currentRenderer()
		// );
	}
}
