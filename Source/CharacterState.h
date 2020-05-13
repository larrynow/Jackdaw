#pragma once
#include"jkInput.h"

class jkCharacter;

// Base state to derive.
class jkCharacterState
{
public:
	virtual ~jkCharacterState() {};
	// Deal a input from current state to a new state.
	virtual jkCharacterState* HandleInput(jkCharacter& character, jkInput& input) = 0;
	virtual void Enter(jkCharacter& character) = 0;
	virtual void Update(jkCharacter& character, const double delta_time) = 0;
};
