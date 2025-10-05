#include "Scoreboard.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "MenuCommands.h"
#include "OpenLevelCommand.h"
#include "ChangeState.h"
#include "InputManager.h"
#include "IntroState.h"
#include "TextRenderer.h"
#include <fstream>
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;

void dae::Scoreboard::Enter()
{
    auto& input = dae::InputManager::GetInstance();

    m_pScene = &dae::SceneManager::GetInstance().GetScene("Game");

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

    // Column titles
    float startY = 50.f;
    float coopX = 10.f;
    float pvpX = 260.f;
    float singleX = 510.f;

    AddText("Coop", coopX, startY, 25);
    AddText("Pvp", pvpX, startY, 25);
    AddText("Single", singleX, startY, 25);

    startY += 40.f;

    const float yOffset = 25.f;
    for (int i = 0; i < 10; ++i)
    {
        // Coop
        if (scoresJson.contains("Coop") && i < scoresJson["Coop"].size()) {
            const auto& entry = scoresJson["Coop"][i];
            std::string name = entry["players"][0].get<std::string>() + " & " + entry["players"][1].get<std::string>();
            std::string line = name + "   " + std::to_string(entry["score"][0].get<int>()) + " & " + std::to_string(entry["score"][1].get<int>());
            AddText(line, coopX, startY + i * yOffset, 20);
        }

        // PvP
        if (scoresJson.contains("Pvp") && i < scoresJson["Pvp"].size()) {
            const auto& entry = scoresJson["Pvp"][i];
            std::string name = entry["players"][0].get<std::string>() + " vs " + entry["players"][1].get<std::string>();
            std::string line = name + "   " + std::to_string(entry["score"][0].get<int>()) + " vs " + std::to_string(entry["score"][1].get<int>());
            AddText(line, pvpX, startY + i * yOffset, 20);
        }

        // Single
        if (scoresJson.contains("Single") && i < scoresJson["Single"].size()) {
            const auto& entry = scoresJson["Single"][i];
            std::string line = entry["name"].get<std::string>() + "   " + std::to_string(entry["score"].get<int>());
            AddText(line, singleX, startY + i * yOffset, 20);
        }
    }

    input.AddAction("GoBack", SDL_SCANCODE_ESCAPE, TriggerEvent::Down, std::make_unique<ChangeState>(std::make_unique<IntroState>()));
    input.AddAction("GoBack", XINPUT_GAMEPAD_B, TriggerEvent::Down, std::make_unique<ChangeState>(std::make_unique<IntroState>()), 1);
}

void dae::Scoreboard::AddText(const std::string& text, float x, float y, int fontSize)
{
    auto textGO = std::make_unique<GameObject>();
    textGO->AddComponent<TextRenderer>();
    textGO->GetComponent<TextRenderer>()->SetFont("Lingua.otf", fontSize);
    textGO->GetComponent<TextRenderer>()->SetText(text);
    textGO->GetTransform()->SetLocalPosition(x, y);
    m_pScene->Add(std::move(textGO));
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