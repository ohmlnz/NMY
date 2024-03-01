#include "Animation.h"
#include "EntityManager.h"
#include <iostream>

Animation::Animation() {};

Animation::Animation(const std::string& animationName, SDL_Texture* texture, Vec2 size, int frame, int speed)
	: m_name(animationName)
	, m_texture(texture)
	, m_maxFrame(frame)
	, m_speed(speed)
	, m_size(size) 
{};

void Animation::update()
{
	if (m_speed > 0)
	{
		m_currentTime++;
	
		if (m_currentTime >= m_speed)
		{
			m_currentFrame = (m_currentFrame + 1) % m_maxFrame;
			m_currentTime = 0;
		}

		m_sprite = {
			static_cast<int>((m_currentFrame) * (m_size.x / m_maxFrame)),
			0,
			static_cast<int>(m_size.x / m_maxFrame),
			static_cast<int>(m_size.y)
		};
	}
};

SDL_Texture* Animation::texture()
{
	return m_texture;
};

const Vec2& Animation::size()
{
	return m_size;
}

const SDL_Rect& Animation::sprite()
{
	return m_sprite;
}

const std::string& Animation::name()
{
	return m_name;
}