#pragma once
#include "BlueTankState.h"
#include <vector>
#include <glm.hpp>

namespace dae
{
    class GridMover;
    class GridNavigation;

    class BlueTankWandering : public BlueTankState
    {
    public:
        BlueTankWandering() = default;
        virtual ~BlueTankWandering() override = default;

        virtual void Enter(GameObject& obj, const std::vector<GridMover*>& players, GridNavigation* nav) override;
        virtual BlueTankState* Update() override;
        virtual void Exit() override;

        void MoveTo(int gx, int gy);
    private:

        void CalculatePath(const glm::ivec2& target);

        // Perform BFS to find the shortest path from start to goal
        std::vector<glm::ivec2> BFSPath(GridNavigation* nav, glm::ivec2 start, glm::ivec2 goal);


        GridNavigation* m_pNavigation = nullptr;

        GridMover* m_pMover = nullptr;
        std::vector<GridMover*> m_pPlayers;

        std::vector<glm::ivec2> m_CurrentPath;
        int m_WaypointIndex = 0;
    };
}

