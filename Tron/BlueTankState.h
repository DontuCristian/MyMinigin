#pragma once
#include <memory>
#include <vector>

namespace dae
{
    // Forward declaration
    class GameObject;
    class GridMover;
	class GridNavigation;

    class BlueTankState
    {
    public:
        BlueTankState() = default;
        virtual ~BlueTankState() = default;

        virtual void Enter(GameObject& obj, const std::vector<GridMover*>& players, GridNavigation* nav) = 0;
        virtual BlueTankState* Update() = 0;
        virtual void Exit() = 0;
    };
}
