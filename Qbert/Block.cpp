#pragma once  
#include "Block.h"  
#include "SpriteRenderer.h"  
#include "GameObject.h"  
#include "PhysicsComponents.h"  
#include "Collisions.h"  
#include "GameLoop.h"
#include "LevelState.h"

dae::Block::Block(GameObject& obj, int nrOfColorChanges, bool revertsBack)
:BComponent(obj),
	m_NrOfColorChanges{nrOfColorChanges},
	m_RevertsBack{revertsBack}
{  
    m_pSpriteRenderer = obj.GetComponent<SpriteRenderer>();
}  

int dae::Block::CollidedWithPlayer()  
{
	if (m_ColorChangeCounter == m_NrOfColorChanges && m_RevertsBack)
	{
		m_pSpriteRenderer->SetRowIdx(m_ColorChangeCounter);
		auto* state = dynamic_cast<LevelState*>(GameLoop::GetInstance().GetCurrentState());

		--m_ColorChangeCounter;

		if (state)
		{
			state->BlockTargetColorReverted();
		}
	}
	else
	{
		++m_ColorChangeCounter;
	}

	if (m_ColorChangeCounter < m_NrOfColorChanges)
	{
		m_pSpriteRenderer->SetRowIdx(m_ColorChangeCounter);
		return 25;
	}
	else if (m_ColorChangeCounter == m_NrOfColorChanges)
	{

		m_pSpriteRenderer->SetRowIdx(m_ColorChangeCounter);
		auto* state = dynamic_cast<LevelState*>(GameLoop::GetInstance().GetCurrentState());

		if (state)
		{
			state->BlockReachedTargetColor();
		}

		return 25;
	}

	return 0;

}

void dae::Block::CollidedWithSlickOrSam()
{
	if (m_ColorChangeCounter == m_NrOfColorChanges)
	{
		--m_ColorChangeCounter;

		m_pSpriteRenderer->SetRowIdx(m_ColorChangeCounter);
		auto* state = dynamic_cast<LevelState*>(GameLoop::GetInstance().GetCurrentState());

		if (state)
		{
			state->BlockTargetColorReverted();
		}
	}
}
