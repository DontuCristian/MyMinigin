#include "IncreaseScoreCommand.h"
#include "GameObject.h"
#include "Score.h"
#include <cassert>

dae::IncreaseScoreCommand::IncreaseScoreCommand(GameObject& obj, int score) :
	CommandObject(obj),
	m_ScoreAmmount{ score }
{
}

void dae::IncreaseScoreCommand::Execute()
{
	assert(GetGameObject()->HasComponent<Score>());

	GetGameObject()->GetComponent<Score>()->IncreaseScore(m_ScoreAmmount);
}
