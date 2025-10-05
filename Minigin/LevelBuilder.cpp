#include "LevelBuilder.h"
#include "GameObject.h"
#include "InputManager.h"
#include "EngineComponentsIncludes.h"
#include "MoveCommand.h"
#include "ShootCommand.h"
#include "ChangeShootingAngle.h"
#include "GridNavigation.h"    
#include "GridMover.h"
#include "BlueTank.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "EnemyHealth.h"

#include <fstream>
#include <SDL.h>
#include <SDL_image.h>
#include "Teleporter.h"
#include "Recognizer.h"
#include "PlayerScore.h"
#include "ScoreHUD.h"
#include "Health.h"
#include "HealthHUD.h"
#include "ServiceLocator.h"
#include "SoundService.h"

#include <random> 

dae::Level dae::LevelBuilder::LoadLevel(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }

    json j;
    file >> j;

    Level level;

	level.mode = j["mode"].get<std::string>();

    for (const auto& playerPos : j["player_spawn"])
    {
		level.player_spawns.push_back(ParseVec2(playerPos));
    }

    // Parse enemy spawns
    for (const auto& enemyJson : j["enemies"]) 
    {
        Enemy enemy;
        enemy.Position.x = enemyJson["pos"][0];
        enemy.Position.y = enemyJson["pos"][1];
        enemy.TankTexture = enemyJson["tank_texture"].get<std::string>();
        enemy.GunTexture = enemyJson["gun_texture"].get<std::string>();

        level.enemies.push_back(std::move(enemy));
    }

    for (const auto& pos : j["recogniser_spawns"])
    {
		level.recogniser_spawns.push_back(ParseVec2(pos));
    }

    // Parse tilemap
    const auto& tile_rows = j["tilemap"];
    for (const auto& row : tile_rows) {
        std::vector<TileType> tile_row;
        for (const auto& tile : row) {
            tile_row.push_back(static_cast<TileType>(tile));
        }
        level.tilemap.push_back(tile_row);
    }

    // Parse teleports
    auto& t = j["teleporter"];
    TeleportLink link;
    link.Position = ParseVec2(t["from"]);

    for (const auto& dest : t["to"]) 
    {
        link.Destinations.push_back(ParseVec2(dest));
	}
    level.teleports.push_back(link);

    for (const auto& col : j["colliders"])
    {
        Collider collider;
        collider.Position = ParseVec2(col["pos"]);
        collider.Size = ParseVec2(col["size"]);
		level.colliders.push_back(collider);
    }

	m_GameplayObjects.player_spawns = level.player_spawns;
	m_GameplayObjects.recogniser_spawns = level.recogniser_spawns;
	m_GameplayObjects.enemies = level.enemies;
	m_GameplayObjects.mode = level.mode;

    m_EnemyCount = static_cast<int>(m_GameplayObjects.enemies.size()) + static_cast<int>(m_GameplayObjects.recogniser_spawns.size());
	m_IsLevelFinished = false;
    return level;
}

void dae::LevelBuilder::BuildLevel(const Level& level, const std::string& sceneName)
{

	auto halfScreenWidth = dae::Renderer::GetInstance().GetWindowWidth() / 2;
	auto halfScreenHeight = dae::Renderer::GetInstance().GetWindowHeight() / 2;

	auto& scene = dae::SceneManager::GetInstance().GetScene(sceneName);
	auto& input = dae::InputManager::GetInstance();

    const int tileSize = 8;

	auto texture = TilemapBatching(level, tileSize);

	auto bigMapObj = std::make_unique<GameObject>();
	bigMapObj->GetTransform()->SetLocalPosition(static_cast<float>(halfScreenWidth), static_cast<float>(halfScreenHeight));
	bigMapObj->AddComponent<dae::TextureRenderer>(texture);

    scene.Add(std::move(bigMapObj));

	auto navGridObj = CreateNavigationGrid(level, tileSize, halfScreenWidth, halfScreenHeight);

    for (auto& collider : level.colliders)
    {
		auto colX = static_cast<float>(collider.Position.x * tileSize + halfScreenWidth - texture->GetSize().x / 2);
		auto colY = static_cast<float>(collider.Position.y * tileSize + halfScreenHeight - texture->GetSize().y / 2);

        auto colliderObject = std::make_unique<GameObject>();
        colliderObject->GetTransform()->SetLocalPosition(colX,colY);
        colliderObject->AddComponent<dae::physics::Collider>(collider.Size.x * tileSize, collider.Size.y * tileSize);
        colliderObject->GetComponent<dae::physics::Collider>()->SetTag(make_sdbm_hash("Wall"));
		scene.Add(std::move(colliderObject));
    }
    


    std::vector<std::unique_ptr<dae::GameObject>> players{};
    std::vector<std::unique_ptr<dae::GameObject>> tankGuns{};
    std::vector<dae::GridMover*> playerGridMovers{};

    for (int idx{}; idx< level.player_spawns.size();++idx)
    {
        auto scoreHUD = std::make_unique<GameObject>();
        scoreHUD->GetTransform()->SetLocalPosition(10, 40 + 40.f * idx);
        scoreHUD->AddComponent<dae::PlayerScore>();
        scoreHUD->AddComponent<dae::ScoreHUD>();
        scoreHUD->GetComponent<dae::ScoreHUD>()->SetFont("Lingua.otf", 20);
        scoreHUD->GetComponent<dae::ScoreHUD>()->SetText("Score: 0");

        auto healthHUD = std::make_unique<GameObject>();
        healthHUD->GetTransform()->SetLocalPosition(10, 60 + 40.f * idx);
        healthHUD->AddComponent<dae::HealthHUD>();
        healthHUD->GetComponent<dae::HealthHUD>()->SetFont("Lingua.otf", 20);
        healthHUD->GetComponent<dae::HealthHUD>()->SetText("Lives: 4");

        // Create player spawn
		players.push_back(std::make_unique<GameObject>());
        players[idx]->GetTransform()->SetLocalPosition(navGridObj->GetComponent<GridNavigation>()->GetGridPosition(level.player_spawns[idx].x, level.player_spawns[idx].y));
        if(idx == 0)
            players[idx]->AddComponent<dae::TextureRenderer>("Tron/Sprites/RedTank.png");
        else
			players[idx]->AddComponent<dae::TextureRenderer>("Tron/Sprites/GreenTank.png");
        players[idx]->AddComponent<dae::physics::Collider>(12.f, 12.f, glm::vec2{ -6.f,-6.f });
        players[idx]->GetComponent<dae::physics::Collider>()->IsTrigger = true;
        players[idx]->AddComponent<dae::GridMover>(navGridObj->GetComponent<GridNavigation>(), static_cast<int>(level.player_spawns[idx].x), static_cast<int>(level.player_spawns[idx].y), 5);
        players[idx]->AddComponent<dae::PlayerScore>();
        players[idx]->GetComponent<dae::PlayerScore>()->AddObserver(scoreHUD->GetComponent<dae::ScoreHUD>());
        players[idx]->AddComponent<dae::Health>(4);
        players[idx]->GetComponent<dae::Health>()->AddObserver(healthHUD->GetComponent<dae::HealthHUD>());

        tankGuns.push_back(std::make_unique<GameObject>());
        tankGuns[idx]->SetParent(players[idx].get(), false);
        if (idx == 0)
            tankGuns[idx]->AddComponent<dae::TextureRenderer>("Tron/Sprites/RedTankGun.png");
        else
			tankGuns[idx]->AddComponent<dae::TextureRenderer>("Tron/Sprites/GreenTankGun.png");


        playerGridMovers.push_back(players[idx]->GetComponent<dae::GridMover>());

        scene.Add(std::move(scoreHUD));
        scene.Add(std::move(healthHUD));
    }

    players[0]->GetComponent<dae::physics::Collider>()->SetTag(make_sdbm_hash("Player1"));

    if(level.player_spawns.size() > 1 )
        if (level.mode == "Pvp")
        {
            players[1]->GetComponent<dae::physics::Collider>()->SetTag(make_sdbm_hash("Player2"));
        }
        else
        {
            players[1]->GetComponent<dae::physics::Collider>()->SetTag(make_sdbm_hash("Player1"));
        }

    //Input binding
    input.AddAction("Shoot0", SDL_SCANCODE_SPACE, dae::TriggerEvent::Down, std::make_unique<dae::ShootCommand>(*tankGuns[0],0));

    input.AddAction("DecreaseShootingAngle0", SDL_SCANCODE_N, dae::TriggerEvent::Pressed, std::make_unique<dae::ChangeShootingAngle>(*tankGuns[0], -180.f));
    input.AddAction("IncreaseShootingAngle0", SDL_SCANCODE_M, dae::TriggerEvent::Pressed, std::make_unique<dae::ChangeShootingAngle>(*tankGuns[0], 180.f));

    input.AddAction("MoveLeft0", SDL_SCANCODE_LEFT, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*players[0], -1, 0));
    input.AddAction("MoveRight0", SDL_SCANCODE_RIGHT, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*players[0], 1, 0));
    input.AddAction("MoveUp0", SDL_SCANCODE_UP, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*players[0], 0, -1));
    input.AddAction("MoveDown0", SDL_SCANCODE_DOWN, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*players[0], 0, 1));

    input.AddAction("MoveLeft0", XINPUT_GAMEPAD_DPAD_LEFT, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*players[0], -1, 0), 0);
    input.AddAction("MoveRight0", XINPUT_GAMEPAD_DPAD_RIGHT, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*players[0], 1, 0), 0);
    input.AddAction("MoveUp0", XINPUT_GAMEPAD_DPAD_UP, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*players[0], 0, -1), 0);
    input.AddAction("MoveDown0", XINPUT_GAMEPAD_DPAD_DOWN, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*players[0], 0, 1), 0);

    input.AddAction("Shoot0", XINPUT_GAMEPAD_A, dae::TriggerEvent::Down, std::make_unique<dae::ShootCommand>(*tankGuns[0],0), 0);

    input.AddAction("DecreaseShootingAngle0", XINPUT_GAMEPAD_X, dae::TriggerEvent::Pressed, std::make_unique<dae::ChangeShootingAngle>(*tankGuns[0], -180.f), 0);
    input.AddAction("IncreaseShootingAngle0", XINPUT_GAMEPAD_B, dae::TriggerEvent::Pressed, std::make_unique<dae::ChangeShootingAngle>(*tankGuns[0], 180.f), 0);


    if (level.player_spawns.size() > 1)
    {
        if(level.mode == "Coop")
            input.AddAction("Shoot1", SDL_SCANCODE_R, dae::TriggerEvent::Down, std::make_unique<dae::ShootCommand>(*tankGuns[1],0));
        else
			input.AddAction("Shoot1", SDL_SCANCODE_R, dae::TriggerEvent::Down, std::make_unique<dae::ShootCommand>(*tankGuns[1], 1));

        input.AddAction("DecreaseShootingAngle1", SDL_SCANCODE_Q, dae::TriggerEvent::Pressed, std::make_unique<dae::ChangeShootingAngle>(*tankGuns[1], -180.f));
        input.AddAction("IncreaseShootingAngle1", SDL_SCANCODE_E, dae::TriggerEvent::Pressed, std::make_unique<dae::ChangeShootingAngle>(*tankGuns[1], 180.f));

        input.AddAction("MoveLeft1", SDL_SCANCODE_A, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*players[1], -1, 0));
        input.AddAction("MoveRight1", SDL_SCANCODE_D, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*players[1], 1, 0));
        input.AddAction("MoveUp1", SDL_SCANCODE_W, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*players[1], 0, -1));
        input.AddAction("MoveDown1", SDL_SCANCODE_S, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*players[1], 0, 1));

        input.AddAction("MoveLeft1", XINPUT_GAMEPAD_DPAD_LEFT, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*players[1], -1, 0), 1);
        input.AddAction("MoveRight1", XINPUT_GAMEPAD_DPAD_RIGHT, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*players[1], 1, 0), 1);
        input.AddAction("MoveUp1", XINPUT_GAMEPAD_DPAD_UP, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*players[1], 0, -1), 1);
        input.AddAction("MoveDown1", XINPUT_GAMEPAD_DPAD_DOWN, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*players[1], 0, 1), 1);

        if (level.mode == "Coop")
            input.AddAction("Shoot1", XINPUT_GAMEPAD_A, dae::TriggerEvent::Down, std::make_unique<dae::ShootCommand>(*tankGuns[1],1), 0);
		else
			input.AddAction("Shoot1", XINPUT_GAMEPAD_A, dae::TriggerEvent::Down, std::make_unique<dae::ShootCommand>(*tankGuns[1], 1), 1);

        input.AddAction("DecreaseShootingAngle1", XINPUT_GAMEPAD_X, dae::TriggerEvent::Pressed, std::make_unique<dae::ChangeShootingAngle>(*tankGuns[1], -180.f), 1);
        input.AddAction("IncreaseShootingAngle1", XINPUT_GAMEPAD_B, dae::TriggerEvent::Pressed, std::make_unique<dae::ChangeShootingAngle>(*tankGuns[1], 180.f), 1);
    }

    for (auto& enemyFile : level.enemies)
    {

        auto enemy = std::make_unique<GameObject>();
        enemy->GetTransform()->SetLocalPosition(navGridObj->GetComponent<GridNavigation>()->GetGridPosition(enemyFile.Position.x, enemyFile.Position.y));
        enemy->AddComponent<dae::physics::Collider>(12.f, 12.f, glm::vec2{ -6.f,-6.f });
        enemy->GetComponent<dae::physics::Collider>()->SetTag(make_sdbm_hash("Enemy"));
        enemy->GetComponent<dae::physics::Collider>()->IsTrigger = true;
        enemy->AddComponent<dae::GridMover>(navGridObj->GetComponent<GridNavigation>(), enemyFile.Position.x, enemyFile.Position.y);
        enemy->AddComponent<dae::BlueTank>(playerGridMovers, navGridObj->GetComponent<GridNavigation>(), enemyFile.TankTexture, enemyFile.GunTexture);
        enemy->AddComponent<dae::EnemyHealth>(false);
        enemy->GetComponent<dae::EnemyHealth>()->AddObserver(&LevelBuilder::GetInstance());
		enemy->GetComponent<dae::EnemyHealth>()->AddObserver(players[0]->GetComponent<dae::PlayerScore>());

        if (level.player_spawns.size() > 1)
            enemy->GetComponent<dae::EnemyHealth>()->AddObserver(players[1]->GetComponent<dae::PlayerScore>());

    
        scene.Add(std::move(enemy));
    }

    for (auto& recog : level.recogniser_spawns)
    {
        auto recogniser = std::make_unique<GameObject>();
        recogniser->GetTransform()->SetLocalPosition(navGridObj->GetComponent<GridNavigation>()->GetGridPosition(recog.x, recog.y));
        recogniser->AddComponent<dae::physics::Collider>(12.f, 12.f, glm::vec2{ -6.f,-6.f });
        recogniser->GetComponent<dae::physics::Collider>()->SetTag(make_sdbm_hash("Recogniser"));
        recogniser->GetComponent<dae::physics::Collider>()->IsTrigger = true;
        recogniser->AddComponent<dae::GridMover>(navGridObj->GetComponent<GridNavigation>(), recog.x, recog.y, 6);
        recogniser->AddComponent<dae::Recognizer>(playerGridMovers, navGridObj->GetComponent<GridNavigation>(), "Tron/Sprites/Recognizer.png");
		recogniser->AddComponent<dae::EnemyHealth>(true);
        recogniser->GetComponent<dae::EnemyHealth>()->AddObserver(&LevelBuilder::GetInstance());
        recogniser->GetComponent<dae::EnemyHealth>()->AddObserver(players[0]->GetComponent<dae::PlayerScore>());

        if (level.player_spawns.size() > 1)
            recogniser->GetComponent<dae::EnemyHealth>()->AddObserver(players[1]->GetComponent<dae::PlayerScore>());

		scene.Add(std::move(recogniser));
    }

    for (auto& teleport : level.teleports)
    {
        auto teleporter = std::make_unique<GameObject>();
        teleporter->GetTransform()->SetLocalPosition(navGridObj->GetComponent<GridNavigation>()->GetGridPosition(teleport.Position.x, teleport.Position.y));
        teleporter->AddComponent<dae::Teleporter>(teleport.Destinations);
		scene.Add(std::move(teleporter));
    }
    scene.Add(std::move(players[0]));
	scene.Add(std::move(tankGuns[0]));
    if (level.player_spawns.size() > 1)
    {
        scene.Add(std::move(players[1]));
        scene.Add(std::move(tankGuns[1]));
	}
    scene.Add(std::move(navGridObj));


}

void dae::LevelBuilder::ResetLevel(const std::string& sceneName)
{
	m_IsLevelFinished = false;
    m_EnemyCount = static_cast<int>(m_GameplayObjects.enemies.size());

    auto& scene = dae::SceneManager::GetInstance().GetScene(sceneName);

    auto navGridObj = scene.Find("Navigation");

    std::vector<GameObject*> players{};
	auto player1 = scene.FindAll("Player1");
	auto player2 = scene.Find("Player2");
    if(!player1.empty())
		players = std::move(player1);
    if(player2)
        players.push_back(scene.Find("Player2"));

    std::vector<dae::GridMover*> playerGridMovers{};


    for (int idx{}; idx < m_GameplayObjects.player_spawns.size(); idx++)
    {
        if (idx > players.size())
        {
            continue;
        }

        if (!players[idx]->GetComponent<dae::Health>()->IsDead())
        {
            players[idx]->GetComponent<dae::GridMover>()->MoveTo(m_GameplayObjects.player_spawns[idx].x, m_GameplayObjects.player_spawns[idx].y);
        }
        playerGridMovers.push_back(players[idx]->GetComponent<dae::GridMover>());
    }

    auto enemies = scene.FindAll("Enemy");
    
    for (int idx{}; idx < m_GameplayObjects.enemies.size(); ++idx)
    {
        if (idx >= enemies.size())
        {
            auto enemy = std::make_unique<GameObject>();
            enemy->GetTransform()->SetLocalPosition(navGridObj->GetComponent<GridNavigation>()->GetGridPosition(m_GameplayObjects.enemies[idx].Position.x, m_GameplayObjects.enemies[idx].Position.y));
            enemy->AddComponent<dae::physics::Collider>(12.f, 12.f, glm::vec2{ -6.f,-6.f });
            enemy->GetComponent<dae::physics::Collider>()->SetTag(make_sdbm_hash("Enemy"));
            enemy->GetComponent<dae::physics::Collider>()->IsTrigger = true;
            enemy->AddComponent<dae::GridMover>(navGridObj->GetComponent<GridNavigation>(), m_GameplayObjects.enemies[idx].Position.x, m_GameplayObjects.enemies[idx].Position.y);
            enemy->AddComponent<dae::BlueTank>(playerGridMovers, navGridObj->GetComponent<GridNavigation>(), m_GameplayObjects.enemies[idx].TankTexture, m_GameplayObjects.enemies[idx].GunTexture);
            enemy->AddComponent<dae::EnemyHealth>(false);
            enemy->GetComponent<dae::EnemyHealth>()->AddObserver(&LevelBuilder::GetInstance());
            enemy->GetComponent<dae::EnemyHealth>()->AddObserver(players[0]->GetComponent<dae::PlayerScore>());

            if (m_GameplayObjects.player_spawns.size() > 1)
                enemy->GetComponent<dae::EnemyHealth>()->AddObserver(players[1]->GetComponent<dae::PlayerScore>());


            scene.Add(std::move(enemy));
            continue;
        }

		enemies[idx]->GetComponent<dae::BlueTank>()->MoveTo(m_GameplayObjects.enemies[idx].Position.x, m_GameplayObjects.enemies[idx].Position.y);
		enemies[idx]->GetComponent<dae::EnemyHealth>()->Reset();
    }
    
    auto recognisers = scene.FindAll("Recogniser");
    for (int idx{}; idx < m_GameplayObjects.recogniser_spawns.size(); ++idx)
    {
        if (idx >= recognisers.size())
        {
            auto recogniser = std::make_unique<GameObject>();
            recogniser->GetTransform()->SetLocalPosition(navGridObj->GetComponent<GridNavigation>()->GetGridPosition(m_GameplayObjects.recogniser_spawns[idx].x, m_GameplayObjects.recogniser_spawns[idx].y));
            recogniser->AddComponent<dae::physics::Collider>(12.f, 12.f, glm::vec2{ -6.f,-6.f });
            recogniser->GetComponent<dae::physics::Collider>()->SetTag(make_sdbm_hash("Recogniser"));
            recogniser->GetComponent<dae::physics::Collider>()->IsTrigger = true;
            recogniser->AddComponent<dae::GridMover>(navGridObj->GetComponent<GridNavigation>(), m_GameplayObjects.recogniser_spawns[idx].x, m_GameplayObjects.recogniser_spawns[idx].y, 6);
            recogniser->AddComponent<dae::Recognizer>(playerGridMovers, navGridObj->GetComponent<GridNavigation>(), "Tron/Sprites/Recognizer.png");
            recogniser->AddComponent<dae::EnemyHealth>(true);
            recogniser->GetComponent<dae::EnemyHealth>()->AddObserver(&LevelBuilder::GetInstance());
            recogniser->GetComponent<dae::EnemyHealth>()->AddObserver(players[0]->GetComponent<dae::PlayerScore>());

            if (m_GameplayObjects.player_spawns.size() > 1)
                recogniser->GetComponent<dae::EnemyHealth>()->AddObserver(players[1]->GetComponent<dae::PlayerScore>());

            scene.Add(std::move(recogniser));
            continue;
        }

		recognisers[idx]->GetComponent<dae::Recognizer>()->MoveTo(m_GameplayObjects.recogniser_spawns[idx].x, m_GameplayObjects.recogniser_spawns[idx].y);
		recognisers[idx]->GetComponent<dae::EnemyHealth>()->Reset();
    }

    auto bullets = scene.FindAll("Bullet");
    for (auto& bullet : bullets)
    {
        bullet->FlagForDeletion();
    }
}

void dae::LevelBuilder::OnNotify(Event event, Subject*)
{
    if(event.id == make_sdbm_hash("EnemyDied"))
    {
		m_EnemyCount--;

        if (m_EnemyCount == 0)
        {
            m_IsLevelFinished = true;
        }
	}
}

std::shared_ptr<dae::Texture2D> dae::LevelBuilder::TilemapBatching(const Level& level, const int tileSize)
{
    SDL_Texture* bigMapTexture =
        SDL_CreateTexture(Renderer::GetInstance().GetSDLRenderer(), SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            static_cast<int>(level.tilemap[0].size() * tileSize),
            static_cast<int>(level.tilemap.size() * tileSize));

    SDL_SetRenderTarget(Renderer::GetInstance().GetSDLRenderer(), bigMapTexture);

    for (size_t y = 0; y < level.tilemap.size(); ++y) {
        for (size_t x = 0; x < level.tilemap[y].size(); ++x) {

            TileType tile = level.tilemap[y][x];
            SDL_Rect dest = { int(x * tileSize), int(y * tileSize), tileSize, tileSize };

            std::string fullPath{};
            switch (tile)
            {
            case dae::TileType::Wall:
                fullPath = "../Data/Tron/Level/wall.png";
                break;
            case dae::TileType::Void:
                fullPath = "../Data/Tron/Level/void.png";
                break;
            case dae::TileType::Path:
                fullPath = "../Data/Tron/Level/path.png";
                break;
            case dae::TileType::Teleport:
                fullPath = "../Data/Tron/Level/teleport.png";
                break;
            default:
                break;
            }
            const auto filename = std::filesystem::path(fullPath).filename().string();

            auto tileTexture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
            SDL_RenderCopy(Renderer::GetInstance().GetSDLRenderer(), tileTexture, NULL, &dest);
        }
    }
    SDL_SetRenderTarget(Renderer::GetInstance().GetSDLRenderer(), NULL);

    auto texture = std::make_shared<dae::Texture2D>(bigMapTexture);

    ResourceManager::GetInstance().AddTexture("BigMap", texture);

    return texture;
}

std::unique_ptr<dae::GameObject> dae::LevelBuilder::CreateNavigationGrid(const Level& level, const int tileSize, int halfWidth, int halfHeight)
{

    const int gridWidth = static_cast<int>(level.tilemap[0].size()) / 2;
    const int gridHeight = static_cast<int>(level.tilemap.size()) / 2;

    auto navGridObj = std::make_unique<GameObject>();
    navGridObj->AddComponent<dae::physics::Collider>(1.f, 1.f, glm::vec2{-100,-100}, true);
    navGridObj->GetComponent<dae::physics::Collider>()->SetTag(make_sdbm_hash("Navigation"));
    navGridObj->AddComponent<dae::GridNavigation>(gridWidth, gridHeight);

    for (int gy = 0; gy < gridHeight; ++gy) {
        for (int gx = 0; gx < gridWidth; ++gx) {

            bool walkable = true;

            // Check the 2x2 tiles that belong to this nav cell
            for (int ty = 0; ty < 2 && walkable; ++ty) {
                for (int tx = 0; tx < 2; ++tx) {
                    int tileX = gx * 2;
                    int tileY = gy * 2;

                    if (tileY >= static_cast<int>(level.tilemap.size()) ||
                        tileX >= static_cast<int>(level.tilemap[tileY].size())) {
                        walkable = false; // Treat out-of-bounds as not walkable
                        break;
                    }

                    TileType tile = level.tilemap[tileY][tileX];
                    if (tile == TileType::Wall || tile == TileType::Void) {
                        walkable = false;
                        break;
                    }
                }
            }

            // Compute nav cell center in world coordinates
            glm::vec2 worldPos{
                gx * 2 * tileSize + (2 * tileSize) / 2.0f + halfWidth - gridWidth * tileSize,
				gy * 2 * tileSize + (2 * tileSize) / 2.0f + halfHeight - gridHeight * tileSize
            };

            navGridObj->GetComponent<dae::GridNavigation>()->SetWalkable(gx, gy, walkable);
            navGridObj->GetComponent<dae::GridNavigation>()->SetPosition(gx, gy, worldPos);
        }
    }

	return navGridObj;
}

glm::vec2 dae::LevelBuilder::ParseVec2(const json& j)
{
    return glm::vec2(j[0].get<float>(), j[1].get<float>());

}
