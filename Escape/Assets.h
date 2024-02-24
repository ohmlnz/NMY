#pragma once
#include <SDL_ttf.h>

struct TextureData
{
	SDL_Texture* texture;
	Vec2 boundingBox;
};

class Assets
{
private:
	TTF_Font* m_font;
	SDL_Color m_color = { 0, 0, 0 };
	SDL_Surface* m_surface;
	const int m_DEFAULT_FONT_SIZE = 30;

public:
	const std::string assetsPath = "./assets.txt";
	const std::string levelPath = "./level.txt";
	void initFont()
	{
		m_font = TTF_OpenFont("./arial.ttf", m_DEFAULT_FONT_SIZE);
	}
	void displayText(const char* text, int x, int y, SDL_Renderer* renderer)
	{
		SDL_Surface* surface = TTF_RenderText_Solid(m_font, text, m_color);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

		int texW = 0;
		int texH = 0;
		SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
		SDL_Rect dstrect = { x, y, texW, texH };

		SDL_RenderCopy(renderer, texture, NULL, &dstrect);
		SDL_RenderPresent(renderer);
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(surface);
	}
	// TODO: abstract texture logic over here
	// void addTexture()
};