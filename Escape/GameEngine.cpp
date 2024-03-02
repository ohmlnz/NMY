#include "GameEngine.h"

void GameEngine::init(int width, int height)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: %s\n" << SDL_GetError() << "\n";
		exit(1);
	}

	m_window = SDL_CreateWindow("Not In My Yard!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
	
	if (m_window == NULL)
	{
		std::cout << "Window could not be created! SDL_Error: %s\n" << SDL_GetError() << "\n";
		exit(1);
	}

	TTF_Init();

	if (TTF_Init() < 0)
	{
		std::cout << "Error from TTF_Init: " << TTF_GetError() << "\n";
		exit(1);
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, 0);
	m_isRunning = true;

	changeScene("Menu", std::make_shared<SceneMenu>(this));
}

void GameEngine::run()
{
	int FRAME_PER_SEC = 60;
	float MS_PER_UPDATE = 1000.0f / FRAME_PER_SEC;
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

		ticksCount = SDL_GetTicks();

		update(deltaTime);

		render();
	}
}

void GameEngine::process()
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

		currentScene()->process(m_event);
	}
}

void GameEngine::update(float deltaTime)
{
	currentScene()->update(deltaTime);
}

void GameEngine::render()
{
	SDL_SetRenderDrawColor(m_renderer, 135, 159, 41, 255);
	SDL_RenderClear(m_renderer);
	currentScene()->render();
	SDL_RenderPresent(m_renderer);
}

void GameEngine::quit()
{
	m_isRunning = false;
	TTF_Quit();
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

SDL_Renderer* GameEngine::currentRenderer()
{
	return m_renderer;
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
	return m_scenes[m_currentScene];
}

SDL_Window* GameEngine::currentWindow()
{
	return m_window;
}

void GameEngine::changeScene(const std::string& name, std::shared_ptr<Scene> scene)
{
	m_scenes[name] = scene;
	m_currentScene = name;
}