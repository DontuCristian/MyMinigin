#include "GameObject.h"
#include "Coily.h"
#include "CoilyBallState.h"
#include "CoilySnakeStateAI.h"

dae::Coily::Coily(GameObject& obj, Transform* playerTransform, bool pvp):
	BComponent(obj),
	m_pPlayerTransform{playerTransform}
{
	m_pCoilyState = new CoilyBallState(pvp);
	m_pCoilyState->Enter(obj, GetOwnerTransform());
}

dae::Coily::~Coily()
{
}

void dae::Coily::Update()
{
	auto* newState = m_pCoilyState->Update();

	if (newState != nullptr)
	{
		m_pCoilyState->Exit();
		m_pCoilyState = newState;
		m_pCoilyState->Enter(*GetOwner(),m_pPlayerTransform);

	}
}

void dae::Coily::Render() const
{
}
