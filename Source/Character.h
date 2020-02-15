#pragma once
#ifndef JKCHARACTER_H_
#define JKCHARACTER_H_

#include "Actor.h"
#include<unordered_map>
#include<functional>
/*
Class jkCharacter : A character is controlled by player thus with a Input_OP map.
*/

class jkCharacter : public jkActor
{
	friend class jkContent;
public:

	jkCharacter() : jkActor() {}
	inline void BindInput(std::string input_name, std::function<void()> operation)
	{
		input_op_map.insert(std::make_pair(input_name, operation));
	};

	virtual void SetUpInput() = 0; // Excute BindInputs in SetInput.

private:

	std::unordered_map<std::string, std::function<void()>> input_op_map;
	// From a player input to a operation.

};

#endif // !JKCHARACTER_H_