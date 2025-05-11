#include "IncreaseScoreCommand.h"
#include "GameObject.h"
#include "PlayerScore.h"
#include <cassert>

dae::IncreaseScoreCommand::IncreaseScoreCommand(GameObject& obj, int score) :
	CommandObject(obj),
	m_ScoreAmmount{ score }
{
}

void dae::IncreaseScoreCommand::Execute()
{
	assert(GetGameObject()->HasComponent<PlayerScore>());

	GetGameObject()->GetComponent<PlayerScore>()->IncreaseScore(m_ScoreAmmount);
}
