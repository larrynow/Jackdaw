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

		BindInput("forward", [this](float v) {MoveForward(v); });//Now pass value(need status checking).
		BindInput("backward", [this](float v) {MoveBack(v); });
		BindInput("left", [this](float v) {MoveLeft(v); });
		BindInput("right", [this](float v) {MoveRight(v); });
		BindInput("up", [this](float v) {MoveUp(v); });
		BindInput("down", [this](float v){MoveDown(v);});

		//BindInput("zoom", [this](float v) {/*ZoomUp(1.f + (v / 120) * 0.1f); */});
		BindInput("turn", [this](float v) {RotateYaw(v); });
		//BindInput("lookup", [this](float v) {/*RotatePitch(v); */});

	}
};

