#include "Scoreboard.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "GameComponentsIncludes.h"
#include "MenuCommands.h"
#include "OpenLevelCommand.h"
#include "ChangeState.h"
#include "InputManager.h"
#include "IntroState.h"
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

void dae::Scoreboard::Enter()
{
    // Create a new scene or reuse existing
    auto& input = dae::InputManager::GetInstance();

    m_pScene = dae::SceneManager::GetInstance().GetScene("Scoreboard");
    if (m_pScene == nullptr)
    {
        dae::SceneManager::GetInstance().CreateScene("Scoreboard");
        m_pScene = dae::SceneManager::GetInstance().GetScene("Scoreboard");
    }

    std::ifstream file("../Data/Score.json");
    if (!file.is_open())
    {
        std::cerr << "Failed to open score file\n";
        return;
    }

    json scoresJson;
    try
    {
        file >> scoresJson;
    }
    catch (const json::parse_error& e)
    {
        std::cerr << "JSON Parse error: " << e.what() << std::endl;
        return;
    }

    // Header text
    auto header = std::make_shared<GameObject>();
    header->AddComponent<TextRenderer>();
    header->GetComponent<TextRenderer>()->SetFont("Lingua.otf", 25);
    header->GetComponent<TextRenderer>()->SetText("Recent Scores");
    header->GetTransform()->SetLocalPosition(200.f, 50.f);
    m_pScene->Add(header);

    const float startX = 200.f;
    float startY = 100.f;
    const float yOffset = 30.f;

    int count = 0;
    for (const auto& entry : scoresJson)
    {
        if (count >= 10) break;

        std::string line = entry.value("name", "???") + ": " + std::to_string(entry.value("score", 0));
        auto scoreGO = std::make_shared<GameObject>();
        scoreGO->AddComponent<TextRenderer>();
        scoreGO->GetComponent<TextRenderer>()->SetFont("Lingua.otf", 20);
        scoreGO->GetComponent<TextRenderer>()->SetText(line);
        scoreGO->GetTransform()->SetLocalPosition(startX, startY);
        m_pScene->Add(scoreGO);

        startY += yOffset;
        ++count;
    }

    input.AddAction("GoBack", SDL_SCANCODE_ESCAPE, TriggerEvent::Down, std::make_unique<ChangeState>(std::make_unique<IntroState>()));
    input.AddAction("GoBack", XINPUT_GAMEPAD_B, TriggerEvent::Down, std::make_unique<ChangeState>(std::make_unique<IntroState>()),1);
}

void dae::Scoreboard::Exit()
{
    auto& input = dae::InputManager::GetInstance();

    input.RemoveAction("GoBack");

    m_pScene->RemoveAll();
}

void dae::Scoreboard::Update()
{
}