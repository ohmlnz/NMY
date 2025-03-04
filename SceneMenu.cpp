#include "SceneMenu.h"

void SceneMenu::process(SDL_Event event)
{
	if (event.type == SDL_KEYDOWN)
	{
		m_gameEngine->changeScene("Main", std::make_shared<SceneMain>(m_gameEngine));
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
	SDL_Color whiteGray = { 255, 247, 247 };

	m_text.setFont("luckiest");
	TTF_Font* currentFont = m_text.getFont();
	TTF_SetFontSize(currentFont, 100);

	m_text.display(
		"Not In My Yard!!",
		(SCREEN_WIDTH / 2) - 320,
		(SCREEN_HEIGHT / 2) - 150,
		m_gameEngine->currentRenderer(),
		whiteGray
	);

	TTF_SetFontSize(currentFont, 30);

	m_text.display(
		"Press any key to start",
		(SCREEN_WIDTH / 2) - 100,
		(SCREEN_HEIGHT / 2) - 30,
		m_gameEngine->currentRenderer(),
		whiteGray
	);
	
};