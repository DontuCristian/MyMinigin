#include "SaveScoreCommand.h"
#include "GameObject.h"
#include "GameLoop.h"
#include "IntroState.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

dae::SaveScoreCommand::SaveScoreCommand(GameObject& obj1, GameObject& obj2, int score1, int score2, const std::string& mode):
    m_Score1{ score1 },
	m_Score2{ score2 },
    m_Mode{ mode }
{
    m_pPlayer1 = obj1.GetComponent<TextInput>();
	m_pPlayer2 = obj2.GetComponent<TextInput>();
}

void dae::SaveScoreCommand::Execute()
{
    if (!m_pPlayer1->IsDone() || !m_pPlayer2->IsDone())
        return;

    auto name1 = m_pPlayer1->GetResult();
	auto name2 = m_pPlayer2->GetResult();
    SaveScore(name1, name2, m_Score1, m_Score2, m_Mode, "../Data/Score.json");

	GameLoop::GetInstance().ChangeState(std::make_unique<IntroState>());
}

void dae::SaveScoreCommand::SaveScore(const std::string& p1, const std::string& p2, int score1, int score2, const std::string& mode, const std::string& path)
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
                scores = json::object(); // fallback to empty
            }
        }
    }

    // Ensure mode category exists
    if (!scores.contains(mode) || !scores[mode].is_array())
        scores[mode] = json::array();

    // Append score
    json entry;
    if (mode == "Single") {
        entry = { {"name", p1}, {"score", score1} };
    }
    else { // Coop or PvP
        entry = { {"players", {p1, p2}}, {"score", {score1,score2}} };
    }

    scores[mode].push_back(entry);

    // Keep only top 10 for that mode
    auto& arr = scores[mode];
    if (arr.size() > 10)
        arr.erase(arr.begin(), arr.begin() + (arr.size() - 10));

    // Save back to file
    std::ofstream outFile(path);
    if (!outFile.is_open())
        throw std::runtime_error("Could not open file for writing: " + path);

    outFile << scores.dump(4);
}
