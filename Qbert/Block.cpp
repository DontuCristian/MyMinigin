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

  m_OnCollisionCallback = std::bind(&Block::UpdateScore, this, std::placeholders::_1);
  obj.GetComponent<physics::Collider>()->SetCollisionCallback(m_OnCollisionCallback);
}  

void dae::Block::UpdateScore(physics::Collision& collision)  
{  
    //Used later 
    collision;

  ++m_ColorChangeCounter;  
  if (m_ColorChangeCounter >= m_NrOfColorChanges)  
  {  
      m_ColorChangeCounter = 0;  
      m_pSpriteRenderer->SetRowIdx(m_ColorChangeCounter + 1);  
  }  
  else  
  {  
      m_pSpriteRenderer->SetRowIdx(m_ColorChangeCounter + 1);  
  }  
}