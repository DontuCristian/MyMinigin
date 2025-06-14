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
    json scores;

    // Load existing scores
    {
        std::ifstream inFile(path);
        if (inFile.is_open())
        {
            try {
                inFile >> scores;
            }
            catch (const json::parse_error& e) {
                std::cerr << "Failed to parse score JSON: " << e.what() << std::endl;
                scores = json::array(); // fallback to empty array
            }
        }
    }

    if (!scores.is_array())
        scores = json::array();

    // Append new score
    scores.push_back({ {"name", name}, {"score", score} });

    // Keep only the last 10
    if (scores.size() > 10)
        scores.erase(scores.begin(), scores.begin() + (scores.size() - 10));

    // Save to file
    std::ofstream outFile(path);
    if (!outFile.is_open())
        throw std::runtime_error("Could not open file for writing: " + path);

    outFile << scores.dump(4);
}
