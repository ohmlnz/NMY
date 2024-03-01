#include "SceneMenu.h"

void SceneMenu::process(SDL_Event event)
{
	switch (event.type)
	{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				updateMenu(m_menuIndex--);
				break;
			case SDLK_DOWN:
				updateMenu(m_menuIndex++);
				break;
			}
		break;
	}
};

void SceneMenu::update(float deltaTime)
{

};

void SceneMenu::render()
{
	for (int i = 0; i <= MAX_INDEX; i++)
	{
		// m_text->displayText(m_menuItems[i], 50, 20 * i * 5, m_gameEngine->currentRenderer());
	}
};

void SceneMenu::updateMenu(int newIndex)
{
	m_menuIndex = newIndex % MAX_INDEX;
}