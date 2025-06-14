#include "SaveScoreCommand.h"
#include "GameObject.h"
#include "GameLoop.h"
#include "IntroState.h"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

dae::SaveScoreCommand::SaveScoreCommand(GameObject& obj, int score):
	CommandObject(obj),
	m_Score{score}
{
	m_TextInput = obj.GetComponent<TextInput>();
}

void dae::SaveScoreCommand::Execute()
{
	if (!m_TextInput->IsDone())
		return;

	auto name = m_TextInput->GetResult();
	SaveScore(name, m_Score, "../Data/Score.json");

	GameLoop::GetInstance().ChangeState(std::make_unique<IntroState>());
}

void dae::SaveScoreCommand::SaveScore(const std::string& name, int score, const std::string& path)
{
	json json{{"name",name},
			 {"score",score}};

	std::ofstream file(path);

	if (!file.is_open())
		throw std::runtime_error("Could not open file for writing: " + path);

	file << json.dump();
}
