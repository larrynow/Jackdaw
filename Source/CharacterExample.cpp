#include "CharacterExample.h"

jkCharacterState* jkExample::FPS::jkStandingState::HandleInput(jkCharacter& character, jkInput& input)
{
	if (!character.CountInput(input))
	{
		return nullptr;
	}

	if (input == jkInput("forward") || input == jkInput("backward") ||
		input == jkInput("left") || input == jkInput("right") ||
		input == jkInput("up") || input == jkInput("down"))
	{
		HandleMovementInput(character, input);

		return new jkMovingState();
	}

	return nullptr;
}

jkCharacterState* jkExample::FPS::jkMovingState::HandleInput(jkCharacter& character, jkInput& input)
{
	// Back to standing state.
	if (!character.CountInput(input))
	{
		return new jkStandingState();
	}

	if (input == jkInput("forward") || input == jkInput("backward") ||
		input == jkInput("left") || input == jkInput("right") || 
		input == jkInput("up") || input == jkInput("down"))
	{
		HandleMovementInput(character, input);

		return this;// Do not change.
	}

	return new jkStandingState();
}

void jkExample::FPS::HandleMovementInput(jkCharacter& character, jkInput& input)
{
	VEC3 additionalMovement;
	if (input == jkInput("forward"))
	{
		additionalMovement = character.GetTransform().GetFront();
	}
	else if (input == jkInput("backward"))
	{
		additionalMovement = -character.GetTransform().GetFront();
	}
	else if (input == jkInput("left"))
	{
		additionalMovement = -character.GetTransform().GetRight();
	}
	else if (input == jkInput("right"))
	{
		additionalMovement = character.GetTransform().GetRight();
	}
	else if (input == jkInput("up"))
	{
		additionalMovement = character.GetTransform().GetUp();
	}
	else if (input == jkInput("down"))
	{
		additionalMovement = -character.GetTransform().GetUp();
	}

	character.ChangeMovingDirection(
		(character.GetMovingDirection() + additionalMovement).Normalize());

}
