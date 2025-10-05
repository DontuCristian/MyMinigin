#pragma once
#include <BaseComponent.h>
#include <vector>
#include <glm.hpp>


namespace dae
{
    struct GridPoint
    {
        glm::vec2 Position;
        bool IsWalkable;
	};

    class GridNavigation : public BComponent
    {
    public:

        GridNavigation(GameObject& obj, int width, int height);

		~GridNavigation() override = default;

        void Update() override;
		void Render() const override;

        void SetWalkable(int x, int y, bool walkable);
        void SetPosition(int x, int y, const glm::vec2& position);

        bool IsWalkable(int x, int y) const;
		glm::vec2 GetGridPosition(int x, int y) const;

        int GetGridWidth() const;
        int GetGridHeight() const;

    private:
		std::vector<std::vector<GridPoint>> m_GridPoints;

    };
}

