#pragma once
#ifndef JKCHARACTER_H_
#define JKCHARACTER_H_

#include "Actor.h"
#include"jkInput.h"
#include"CharacterState.h"
#include<unordered_map>
#include<unordered_set>
#include<functional>
/*
Class jkCharacter : A character is controlled by player thus with a Input_OP map.
*/

class jkCharacter : public jkActor
{
	friend class jkContent;
public:

	jkCharacter() : jkActor()
	{
		// Register input in derived classes : SetUpInput().
	}

	virtual ~jkCharacter() { if(m_pCurrentState) delete m_pCurrentState; }

	inline void BindInput(std::string input_name, std::function<void(float)> operation)
	{
		input_op_map.insert(std::make_pair(input_name, operation));
	}

	inline void RegisterInput(std::string input_name)
	{
		mInputMap.insert(jkInput(input_name));
	}
	inline bool CountInput(jkInput& input)
	{
		return mInputMap.count(input);
	}

	virtual void SetUpInput() = 0; // Excute BindInputs in SetInput.

	virtual void HandleInput(jkInput& input);

	virtual void Update(const double delta_time)
	{
		if(m_pCurrentState)
			m_pCurrentState->Update(*this, delta_time);
	}

protected:

	std::unordered_map<std::string, std::function<void(float)>> input_op_map;
	// Input porotypes without value.
	std::unordered_set<jkInput, jkInput::jkInputHash> mInputMap;
	jkCharacterState* m_pCurrentState;
	// From a player input to a operation.

};

#endif // !JKCHARACTER_H_