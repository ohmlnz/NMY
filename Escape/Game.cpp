#include "Game.h"
#include "Assets.h"

void Game::init(int width, int height, const std::string& assetsPath, const std::string& levelPath)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}

	m_window = SDL_CreateWindow("Not On My Yard!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
	
	if (m_window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}


	TTF_Init();

	m_renderer = SDL_CreateRenderer(m_window, -1, 0);
	m_isRunning = true;

	m_scene = std::make_shared<Scene>(this);
	m_scene->loadLevel(assetsPath, levelPath);
}

void Game::run()
{
	const int FRAME_PER_SEC = 60;
	const float MS_PER_UPDATE = 1000.0f / FRAME_PER_SEC;
	Uint32 ticksCount = 0;

	while (m_isRunning)
	{
		process();

		while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksCount + MS_PER_UPDATE));

		float deltaTime = (SDL_GetTicks() - ticksCount) / 1000.0f;

		if (deltaTime > 0.05f)
		{
			deltaTime = 0.05f;
		}

		update(deltaTime);

		render();
	}
}

void Game::process()
{
	while (SDL_PollEvent(&m_event))
	{
		switch (m_event.type)
		{
			case SDL_QUIT:
				quit();
				break;
			default:
				break;
		}

		m_scene->handleEvents(m_event);
	}
}

void Game::update(float deltaTime)
{
	m_scene->update(deltaTime);
}

void Game::render()
{
	SDL_SetRenderDrawColor(m_renderer, 135, 159, 41, 255);
	SDL_RenderClear(m_renderer);
	m_scene->render();
	SDL_RenderPresent(m_renderer);
}

void Game::quit()
{
	m_isRunning = false;
	TTF_Quit();
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

SDL_Renderer* Game::currentRenderer()
{
	return m_renderer;
}

std::shared_ptr<Scene> Game::currentScene()
{
	return m_scene;
}