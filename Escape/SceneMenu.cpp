#include "SceneMenu.h"

void SceneMenu::process(SDL_Event event)
{
	switch (event.type)
	{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_RETURN:
				if (m_menuIndex == 0)
				{
					m_gameEngine->changeScene("Main", std::make_shared<SceneMain>(m_gameEngine));
				}
				break;
			case SDLK_UP:
				m_menuIndex--;
				break;
			case SDLK_DOWN:
				m_menuIndex++;
				break;
			}
		break;
	}
};

void SceneMenu::update(float deltaTime)
{
	if (m_menuIndex < 0)
	{
		m_menuIndex = MAX_INDEX;
	}
	else if (m_menuIndex > MAX_INDEX)
	{
		m_menuIndex = 0;
	}
};

void SceneMenu::render()
{
	for (int i = 0; i <= MAX_INDEX; i++)
	{
		bool isSelected = i == m_menuIndex;
		SDL_Color red = { 255, 0, 0 };
		SDL_Color white = { 0, 0, 0 };
		m_text.displayText(m_menuItems[i], 50, 20 * i * 5, m_gameEngine->currentRenderer(), isSelected ? red : white);
	}
};