#pragma once
#include "Vec2.h"
#include <string>

// TODO: refactor such that there is one struct per entity type, with a base struct that is common to all of them

struct Entity
{
private:
	size_t			m_id = 0;
	std::string		m_tag = "Default";
	bool			m_active = true;
	std::string		m_texture;

public:
	Entity(const std::string& tag, const std::string& texture, Vec2 position, Vec2 size)
		: m_tag(tag)
		, m_texture(texture)
		, m_position(position)
		, m_size(size)
		, m_half(size.x / 2, size.y / 2)
	{}
	void destroy() { m_active = false; }
	bool isActive() { return m_active; }
	size_t& id() { return m_id; }
	void setId(size_t id) { m_id = id; }
	std::string& tag() { return m_tag; }
	std::string texture() { return m_texture; }		
	double	m_angle = 0;

	// transform
	Vec2 m_position = { 0, 0 };
	Vec2 m_scale = { 1, 1 };
	Vec2 m_overlap = { 0, 0 };
	Vec2 m_previousPosition = { 0, 0 };

	// bounding box
	Vec2 m_size = { 0, 0 };
	Vec2 m_half = { 0, 0 };
};

struct Playable : public Entity
{
public:
	Playable(const std::string& tag, const std::string& texture, Vec2 position, Vec2 size) : 
		Entity(tag, texture, position, size) {}
	// TODO: initialize at creation
	Vec2 m_velocity = { 0.2, 0.2 };
	int  m_cooldown = 1;

	// state
	enum State {
		IDLING,
		MOVING,
	};

	// TODO: maybe better for this to be an enum? 
	bool m_up = false;
	bool m_left = false;
	bool m_right = false;
	bool m_down = false;

	// TODO: not sure if string is the best type here
	std::string m_direction = "right";
	State m_currentState = State::IDLING;
	// State m_previousState;
};

struct Obstacle : public Entity
{
public:
	Obstacle(const std::string& tag, const std::string& texture, Vec2 position, Vec2 size) :
		Entity(tag, texture, position, size) {}
};

struct Decoration : public Entity
{
public:
	Decoration(const std::string& tag, const std::string& texture, Vec2 position, Vec2 size) :
		Entity(tag, texture, position, size) {}
};