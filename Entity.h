#pragma once
#include <string>

#include "Vec2.h"
#include "Animation.h"

enum State {
	IDLING,
	MOVING,
};

struct Entity
{
private:
	size_t			m_id = 0;
	std::string		m_tag = "Default";
	bool			m_active = true;

public:
	Entity(const std::string& tag, Animation animation, Vec2 position, Vec2 size, Vec2 boundingBox)
		: m_tag(tag)
		, m_animation(animation)
		, m_position(position)
		, m_size(size)
		, m_half(size.x / 2, size.y / 2)
		, m_boundingBox(boundingBox)
	{}
	void destroy() { m_active = false; }
	bool isActive() { return m_active; }
	size_t& id() { return m_id; }
	void setId(size_t id) { m_id = id; }
	std::string& tag() { return m_tag; }
	bool m_flipped = false;
	double	m_angle = 0;
	Animation m_animation;

	// transform
	Vec2 m_position = { 0, 0 };
	Vec2 m_previousPosition = { 0, 0 };
	Vec2 m_scale = { 1, 1 };
	Vec2 m_overlap = { 0, 0 };
	Vec2 m_previousOverlap = { 0, 0 };

	Vec2 m_size = { 0, 0 };
	Vec2 m_half = { 0, 0 };

	// bounding box
	Vec2 m_boundingBox = { 0, 0 };
};

struct Playable : public Entity
{
public:
	Playable(const std::string& tag, Animation animation, Vec2 position, Vec2 size, Vec2 boundingBox, Vec2 velocity) :
		Entity(tag, animation, position, size, boundingBox),
		m_velocity(velocity)
	{}
	// TODO: initialize at creation
	Vec2 m_velocity = { 0.2, 0.2 };
	int  m_cooldown = 1;

	// TODO: maybe better for this to be an enum? 
	bool m_up = false;
	bool m_left = false;
	bool m_right = false;
	bool m_down = false;

	// TODO: not sure if string is the best type here
	std::string m_direction = "right";
	State m_currentState = State::IDLING;
	// State m_previousState;

	// flag used to reset position of blower
	bool m_wasPositionReset = false;
};

struct Obstacle : public Entity
{
public:
	Obstacle(const std::string& tag, Animation animation, Vec2 position, Vec2 size, Vec2 boundingBox) :
		Entity(tag, animation, position, size, boundingBox) {}
};

struct Decoration : public Entity
{
public:
	Decoration(const std::string& tag, Animation animation, Vec2 position, Vec2 size, Vec2 boundingBox) :
		Entity(tag, animation, position, size, boundingBox) {}
};