#pragma once
#include <BaseComponent.h>
#include "GridNavigation.h"

namespace dae
{
    class GridMover : public BComponent
    {

    public:

        GridMover(GameObject& obj, GridNavigation* nav, int gx, int gy, int speed = {3});
        void Update() override;
        void Render() const override {};

		void MoveBy(int dx, int dy);
		void MoveTo(int gx, int gy);

		glm::ivec2 GetGridPosition() const { return glm::ivec2{ m_GridX, m_GridY }; }

		bool IsMoving() const { return m_Moving; }

    private:
		Transform* m_pTransform{};
        GridNavigation* m_Navigation{};
		int m_GridX;
		int m_GridY;

        glm::vec2 m_CurrentPos{};
        glm::vec2 m_TargetPos{};
        float m_Progress = 0.0f;
        int m_MoveSpeed {3}; // cells per second
        bool m_Moving = false;
    };
}

