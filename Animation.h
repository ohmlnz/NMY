#pragma once
#include <SDL_image.h>
#include "Vec2.h"
#include <string>

class Animation
{
private:
	int m_frameCount = 0;
	int m_maxFrame = 0;
	int m_currentFrame = 0;
	float m_currentTime = 0.0f;
	int m_speed = 0;
	SDL_Texture* m_texture = nullptr;
	Vec2 m_size = { 0, 0 };
	std::string m_name = "Default";
	SDL_Rect m_sprite;
public:
	Animation();
	Animation(const std::string& animationName, SDL_Texture* texture, Vec2 size, int frame, int speed);
	void update();
	SDL_Texture* texture();
	const Vec2& size();
	const SDL_Rect& sprite();
	const std::string& name();
};