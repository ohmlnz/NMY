#pragma once
#include <SDL_ttf.h>
#include <iostream>

class Text
{
private:
	TTF_Font* m_font;
	SDL_Texture* m_texture;
	int m_DEFAULT_FONT_SIZE = 30;

public:
	~Text()
	{
		SDL_DestroyTexture(m_texture);
		TTF_CloseFont(m_font);
	}

	void initFonts()
	{
		m_font = TTF_OpenFont("./arial.ttf", m_DEFAULT_FONT_SIZE);

		if (!m_font)
		{
			std::cout << "Failed to load font: " << TTF_GetError() << "\n";
		}
	}

	void displayText(const char* text, int x, int y, SDL_Renderer* renderer, SDL_Color color = { 0, 0, 0 }, int fontSize = 30)
	{
		TTF_SetFontSize(m_font, fontSize);
		SDL_Surface* surface = TTF_RenderUTF8_Solid(m_font, text, color);
		SDL_Rect area = { x, y, surface->w, surface->h };
		m_texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_RenderCopy(renderer, m_texture, NULL, &area);
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(m_texture);
	}
};