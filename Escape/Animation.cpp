#include "Animation.h"
#include "EntityManager.h"

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
