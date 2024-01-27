#pragma once
#include "Entity.h"

struct Player : public Entity
{
public:
	// state
	enum State {
		IDLING,
		MOVING,
	};

	bool m_up = false;
	bool m_left = false;
	bool m_right = false;
	bool m_down = false;

	State m_currentState = State::IDLING;
	// State m_previousState;
};