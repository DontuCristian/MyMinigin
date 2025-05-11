#pragma once  
#include "Block.h"  
#include "SpriteRenderer.h"  
#include "GameObject.h"  
#include "PhysicsComponents.h"  
#include "Collisions.h"  

dae::Block::Block(GameObject& obj)  
: BComponent(obj)  
{  
    m_pSpriteRenderer = obj.GetComponent<SpriteRenderer>();
}  

int dae::Block::CollidedWithPlayer()  
{
    ++m_ColorChangeCounter;

	if (m_ColorChangeCounter < m_NrOfColorChanges)
	{
		m_pSpriteRenderer->SetRowIdx(m_ColorChangeCounter);
		return 15;
	}
	else if (m_ColorChangeCounter == m_NrOfColorChanges)
	{
		m_pSpriteRenderer->SetRowIdx(m_ColorChangeCounter);
		return 30;
	}

	return 0;

}