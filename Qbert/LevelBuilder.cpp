#include "LevelBuilder.h"
#include "GameObject.h"
#include "InputManager.h"
#include "GameComponentsIncludes.h"
#include "EngineComponentsIncludes.h"
#include "MoveCommand.h"

#include <fstream>


void dae::LevelBuilder::BuildLevel(const std::string& filePath, Scene* scene)
{
    std::ifstream file(filePath);
    if (!file.is_open()) return;

    json levelData;
    file >> levelData;

    glm::vec2 pyramidStart{};
    float cubeSize{};
    int totalRows{};
    float offset{};

    BuildPyramid(levelData["pyramid"], scene, pyramidStart, cubeSize, totalRows, offset);
    AddKillZones(scene, pyramidStart, cubeSize, totalRows, offset);

    if (levelData.contains("elevators"))
        AddElevator(levelData["elevators"], scene);

    if (levelData.contains("players"))
    {
        auto players = AddPlayers(levelData["players"], scene);
        AddUI(scene, players);
    }
}

void dae::LevelBuilder::BuildPyramid(const json& pyramidData, Scene* scene, glm::vec2& outStartPos, float& outCubeSize, int& outTotalRows, float& outOffset)
{
    outTotalRows = pyramidData["rows"];
    outCubeSize = pyramidData["blockSize"];
    outOffset = pyramidData["offset"];
    outStartPos = { pyramidData["startPosition"][0], pyramidData["startPosition"][1] };

    int colorChanges = pyramidData["properties"]["colorChanges"];
    bool reverts = pyramidData["properties"]["revertsBack"];

    auto spriteSheet = pyramidData["sprites"]["sheet"];
    int sheetRows = pyramidData["sprites"]["rows"];
    int sheetCols = pyramidData["sprites"]["cols"];
    int outsideFrame = pyramidData["sprites"]["outsideFrame"];
    int insideFrame = pyramidData["sprites"]["insideFrame"];

    for (int row = 0; row < outTotalRows; ++row) {
        for (int col = 0; col <= row; ++col) {
            bool isOutside = (col == 0 || col == row);

            auto block = std::make_shared<GameObject>();
            block->AddComponent<dae::SpriteRenderer>();
            block->GetComponent<dae::SpriteRenderer>()->SetSpriteSheet(spriteSheet, sheetRows, sheetCols);
            block->GetComponent<dae::SpriteRenderer>()->SetFrameIdx(isOutside ? outsideFrame : insideFrame);

            float x = outStartPos.x + (col - row * 0.5f) * outCubeSize;
            float y = outStartPos.y - (-row * (outCubeSize - outOffset));
            block->GetTransform()->SetLocalPosition(x, y);

            block->AddComponent<dae::physics::Collider>(16.f, 16.f, glm::vec2{ -8.f , 0.f });
            block->GetComponent<dae::physics::Collider>()->SetTag("Block");
            block->AddComponent<dae::Block>(colorChanges,reverts);
            block->GetComponent<dae::Block>()->SetIsEdge(row == outTotalRows - 1);

            scene->Add(block);
        }
    }
}

void dae::LevelBuilder::AddKillZones(Scene* scene, const glm::vec2& startPos, float cubeSize, int totalRows, float offset)
{
    glm::vec2 deathStartPos = startPos - glm::vec2{ offset, cubeSize };

    for (int row = 1; row < totalRows + 2; ++row) {
        for (int col = 0; col <= row; ++col) {
            if (col == 0 || col == row) {
                auto killZone = std::make_shared<GameObject>();

                float x = deathStartPos.x + (col - row * 0.5f) * cubeSize;
                float y = deathStartPos.y - (-(row - 1) * (cubeSize - offset));
                x += (col == 0 ? -cubeSize / 4.f : cubeSize / 4.f);

                killZone->GetTransform()->SetLocalPosition(x, y);
                killZone->AddComponent<dae::physics::Collider>(42.f, 16.f, glm::vec2{ -8.f ,20.f }, true);
                killZone->GetComponent<dae::physics::Collider>()->SetTag("KillZone");

                scene->Add(killZone);
            }
        }
    }
}

std::vector<std::shared_ptr<dae::GameObject>> dae::LevelBuilder::AddPlayers(const json& playersData, Scene* scene)
{
    std::vector<std::shared_ptr<GameObject>> players;

    for (const auto& playerData : playersData) {
        glm::vec2 pos{ playerData["position"][0], playerData["position"][1] };

        auto player = std::make_shared<GameObject>();
        player->GetTransform()->SetLocalPosition(pos);

        player->AddComponent<SpriteRenderer>();
        player->GetComponent<SpriteRenderer>()->SetSpriteSheet("QBertMoving.png", 4, 2);
        player->GetComponent<SpriteRenderer>()->SetFrameDelay(0.3f);

        player->AddComponent<dae::physics::RigidBody>();
        player->AddComponent<dae::physics::Collider>(16.f, 16.f, glm::vec2{ -8.f,12.f });
        player->GetComponent<dae::physics::Collider>()->SetTag("Player");

        player->AddComponent<DeathHandlerComponent>();
        player->AddComponent<Health>();
        player->GetComponent<Health>()->AddObserver(player->GetComponent<DeathHandlerComponent>());
        player->AddComponent<PlayerScore>();

        scene->Add(player);

        players.push_back(player);

    }
    auto& input = InputManager::GetInstance();

    const auto& controls0 = playersData[0]["controls"];
    input.AddAction("MoveUpLeft", controls0["upLeft"], dae::TriggerEvent::Down, std::make_unique<MoveCommand>(*players[0].get(), glm::vec2{-1,-5.4}, 250.f));
    input.AddAction("MoveDownRight", controls0["downRight"], dae::TriggerEvent::Down, std::make_unique<MoveCommand>(*players[0].get(), glm::vec2{ 1,-2.5 }, 120.5f));
    input.AddAction("MoveUpRight", controls0["upRight"], dae::TriggerEvent::Down, std::make_unique<MoveCommand>(*players[0].get(), glm::vec2{ 1,-5.4 }, 250.f));
    input.AddAction("MoveDownLeft", controls0["downLeft"], dae::TriggerEvent::Down, std::make_unique<MoveCommand>(*players[0].get(), glm::vec2{ -1,-2.5 }, 120.5f));

    if (players.size() >= 2)
    {
        const auto& controls1 = playersData[1]["controls"];
        input.AddAction("MoveUpLeft", controls1["upLeft"], dae::TriggerEvent::Down, std::make_unique<MoveCommand>(*players[1].get(), glm::vec2{ -1,-5.4 }, 250.f), 1);
        input.AddAction("MoveDownRight", controls1["downRight"], dae::TriggerEvent::Down, std::make_unique<MoveCommand>(*players[1].get(), glm::vec2{ 1,-2.5 }, 120.5f), 1);
        input.AddAction("MoveUpRight", controls1["upRight"], dae::TriggerEvent::Down, std::make_unique<MoveCommand>(*players[1].get(), glm::vec2{ 1,-5.4 }, 250.f), 1);
        input.AddAction("MoveDownLeft", controls1["downLeft"], dae::TriggerEvent::Down, std::make_unique<MoveCommand>(*players[1].get(), glm::vec2{ -1,-2.5 }, 120.5f), 1);
    }

    return players;
}

void dae::LevelBuilder::AddElevator(const json& elevatorsData, Scene* scene)
{
    for (const auto& elevatorData : elevatorsData) {
        auto elevator = std::make_shared<GameObject>();
        elevator->GetTransform()->SetLocalPosition(elevatorData["position"][0], elevatorData["position"][1]);

        elevator->AddComponent<SpriteRenderer>();
        elevator->GetComponent<SpriteRenderer>()->SetSpriteSheet(elevatorData["spriteSheet"], 1, elevatorData["cols"]);
        elevator->GetComponent<SpriteRenderer>()->SetFrameDelay(elevatorData["frameDelay"]);

        elevator->AddComponent<dae::physics::Collider>(32.f, 10.f, glm::vec2{ -16.f,-5.f }, true);
        elevator->GetComponent<dae::physics::Collider>()->SetTag("Elevator");
        elevator->AddComponent<Platform>();

        scene->Add(elevator);
    }
}

void dae::LevelBuilder::AddUI(Scene* scene, const std::vector<std::shared_ptr<GameObject>>& players)
{
    // Global FPS counter
    auto fpsDisplay = std::make_shared<GameObject>();
    fpsDisplay->AddComponent<FPSRenderer>();
    fpsDisplay->GetComponent<TextRenderer>()->SetFont("Lingua.otf", 25);
    fpsDisplay->GetTransform()->SetLocalPosition(20.f, 30.f);
    scene->Add(fpsDisplay);

    const float baseX = 30.f;
    float currentY = 70.f;
    const float yOffset = 40.f;

    for (size_t i = 0; i < players.size(); ++i) {
        const auto& player = players[i];

        // Health HUD
        auto healthHUD = std::make_shared<GameObject>();
        healthHUD->AddComponent<HealthHUD>();
        healthHUD->GetComponent<HealthHUD>()->SetFont("Lingua.otf", 15);
        healthHUD->GetComponent<HealthHUD>()->SetText("Lives: 3");
        healthHUD->GetTransform()->SetLocalPosition(baseX, currentY);
        scene->Add(healthHUD);

        // Score HUD
        auto scoreHUD = std::make_shared<GameObject>();
        scoreHUD->AddComponent<ScoreHUD>();
        scoreHUD->GetComponent<ScoreHUD>()->SetFont("Lingua.otf", 15);
        scoreHUD->GetComponent<ScoreHUD>()->SetText("0");
        scoreHUD->GetTransform()->SetLocalPosition(baseX, currentY + 20.f);
        scene->Add(scoreHUD);

        // Observer hookups
        if (player->HasComponent<Health>())
            player->GetComponent<Health>()->AddObserver(healthHUD->GetComponent<HealthHUD>());

        if (player->HasComponent<PlayerScore>())
            player->GetComponent<PlayerScore>()->AddObserver(scoreHUD->GetComponent<ScoreHUD>());

        currentY += yOffset;
    }
}
