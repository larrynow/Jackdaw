#pragma once
#include "Character.h"
class jkFreeCamera :
	public jkCharacter
{
	/*
	* jkFreeCamera : a character class act like a camera.
	*/

public:
	jkFreeCamera() :jkCharacter() 
	{
		SetUpInput();
	}

	void SetUpInput() override
	{
		////////////////////////////////////////////
		// Bind default inputs for a free camera.

		BindInput("forward", [this]() {MoveForward(); });
		BindInput("backward", [this]() {MoveBack(); });
		BindInput("left", [this]() {MoveLeft(); });
		BindInput("right", [this]() {MoveRight(); });
		BindInput("up", [this]() {MoveUp(); });
		BindInput("down", [this](){MoveDown();});
	}
};

