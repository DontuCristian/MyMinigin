#pragma once
#include <Singleton.h>
#include <Observer.h>
#include <Event.h>
#include <Subject.h>
#include "Scene.h"
#include "json.hpp"
#include "glm.hpp"
#include <string>
#include <memory>

namespace dae
{
	class Texture2D;
	class GameObject;

    using json = nlohmann::json;

    enum class TileType
    {
        Wall,
        Void,
        Path,
        Teleport
	};

    struct Enemy
    {
        glm::ivec2 Position{};
        std::string TankTexture{};
        std::string GunTexture{};
	};
    struct TeleportLink
    {
        glm::ivec2 Position{};
        std::vector<glm::ivec2> Destinations{};
	};

    struct Collider
    {
        glm::vec2 Position{};
        glm::vec2 Size{};
    };

    struct Level 
    {
        std::string mode{};
        std::vector<std::vector<TileType>> tilemap{};
        std::vector<glm::ivec2> player_spawns{};
        std::vector<glm::ivec2> recogniser_spawns{};
        std::vector<Enemy> enemies{};
        std::vector<TeleportLink> teleports;
        std::vector<Collider> colliders{};
    };


    struct GameplayObjects
    {
        std::string mode{};
        std::vector<glm::ivec2> player_spawns{};
        std::vector<glm::ivec2> recogniser_spawns{};
        std::vector<Enemy> enemies{};
    };

	class LevelBuilder final : public Singleton<LevelBuilder>, public Observer
	{
    public:
        Level LoadLevel(const std::string& filePath);

        void BuildLevel(const Level& level, const std::string& sceneName);
		void ResetLevel(const std::string& sceneName);

        bool IsLevelFinished() { return m_IsLevelFinished; }

        void OnNotify(Event event, Subject* subject) override;

    private:
        friend class Singleton<SceneManager>;

        std::shared_ptr<Texture2D> TilemapBatching(const Level& level, const int tileSize);
		std::unique_ptr<GameObject> CreateNavigationGrid(const Level& level, const int tileSize, int halfWidth, int halfHeight);
        glm::vec2 ParseVec2(const json& j);

		GameplayObjects m_GameplayObjects;

		int m_EnemyCount{ 0 };

		bool m_IsLevelFinished{ false };

	};
}

