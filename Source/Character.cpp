#include "Character.h"

void jkCharacter::HandleInput(jkInput& input)
{
	auto state = m_pCurrentState->HandleInput(*this, input);
	if (state != nullptr && state!=m_pCurrentState)
	{
		delete m_pCurrentState;
		m_pCurrentState = state;
		m_pCurrentState->Enter(*this);
	}
}
