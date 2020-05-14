#pragma once
#include "../Character.h"

namespace jkExample
{
	namespace FPS
	{
		/////////////////////////////
		// FPS example states.

		// Movements.
		class jkStandingState : public jkCharacterState
		{
		public:
			virtual jkCharacterState* HandleInput(jkCharacter& character, jkInput& input) override;


			virtual void Enter(jkCharacter& character) override
			{
				// Stop moving animation();
				character.ChangeMovingDirection(VEC3());// Stop moving.
			}
			virtual void Update(jkCharacter& character, const double delta_time) override
			{
				// Do nothing.
			}
		};

		class jkMovingState : public jkCharacterState
		{
		public:
			virtual jkCharacterState* HandleInput(jkCharacter& character, jkInput& input) override;

			virtual void Enter(jkCharacter& character) override
			{
				//Change to moving animation.
			}
			virtual void Update(jkCharacter& character, const double delta_time) override
			{
				character.UpdateMovement(delta_time);
				// Play the moving animation.
				character.PlayAnimation(std::string("moving"));
			}
		};

		////////////////////////////////////
		// FPS character class example.

		class jkCharacterExample :
			public jkCharacter
		{
		public:
			jkCharacterExample() :jkCharacter()
			{
				SetUpInput();
				m_pCurrentState = new jkStandingState();
			}

			void SetUpInput() override
			{
				////////////////////////////////////////////
				// Bind default inputs for a free camera.

				/*
				BindInput("forward", [this](float v) {MoveForward(v); });//Now pass value(need status checking).
				BindInput("backward", [this](float v) {MoveBack(v); });
				BindInput("left", [this](float v) {MoveLeft(v); });
				BindInput("right", [this](float v) {MoveRight(v); });
				BindInput("up", [this](float v) {MoveUp(v); });
				BindInput("down", [this](float v){MoveDown(v);});
				*/
				//BindInput("zoom", [this](float v) {/*ZoomUp(1.f + (v / 120) * 0.1f); */});
				//BindInput("turn", [this](float v) {RotateYaw(v); });
				//BindInput("lookup", [this](float v) {/*RotatePitch(v); */});

				RegisterInput("forward");
				RegisterInput("backward");
				RegisterInput("left");
				RegisterInput("right");
				RegisterInput("up");
				RegisterInput("down");

				RegisterInput("turn");

			}
		};

		

		// Change character movement with inputs.
		void HandleMovementInput(jkCharacter& character, jkInput& input);
		

		
	}
	
}

