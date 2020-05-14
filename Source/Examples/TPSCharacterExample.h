#pragma once
#include "../Character.h"

namespace jkExample
{
	namespace TPS
	{
		class TPSCharacterExample : public jkCharacter
		{
			/////////////////////////////
			// TPS example states.

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
					//PlayAnimation();
				}
			};
		};
	}
}


