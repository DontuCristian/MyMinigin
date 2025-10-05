#pragma once
#include <BaseComponent.h>
#include <vector>
#include <glm.hpp>

namespace dae
{
    namespace physics
    {
        struct RigidBody;
        struct Collider;
        struct CollisionPoints;
    }

	class Transform;
	class GridMover;
	class GridNavigation;
	class Texture2D;

    class Recognizer : public BComponent
    {
    public:
        Recognizer(GameObject& obj, const std::vector<GridMover*>& players, GridNavigation* nav, const std::string& tankpath);

        void Update() override;
        void Render() const override;

        void MoveTo(int gx, int gy);

    private:
        void RecalculatePath(const glm::ivec2& playerPos);

        void OnTrigger(const physics::Collider* other, const physics::CollisionPoints& points);

        // Perform BFS to find the shortest path from start to goal
        std::vector<glm::ivec2> BFSPath(GridNavigation* nav, glm::ivec2 start, glm::ivec2 goal);

        Transform* m_pTransform = nullptr;
        std::shared_ptr<Texture2D> m_pTankTexture = nullptr;


        dae::GridNavigation* m_pNavigation = nullptr;
        GridMover* m_pMover = nullptr;
        std::vector<GridMover*> m_pPlayers;


        glm::ivec2 m_LastKnownPlayerPos{};
        float m_PathCooldown = 0.25f; // seconds
        float m_PathTimer = 0.0f;
        std::vector<glm::ivec2> m_CurrentPath;
        int m_WaypointIndex = 0;
    };
}

