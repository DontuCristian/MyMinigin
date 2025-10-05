#pragma once
#include <BaseComponent.h>
#include "GridNavigation.h"
#include "GridMover.h"

namespace dae
{
	class GridNavigation;
	class GridMover;
	class Transform;
	class Texture2D;
	class BlueTankState;

    class BlueTank : public BComponent
    {

	public:

        BlueTank(GameObject& obj, const std::vector<GridMover*>& players, GridNavigation* nav, const std::string& tankpath, const std::string& gunpath);

        void Update() override;
		void Render() const override;

		void MoveTo(int gx, int gy);
    private:

        bool CanSeePlayer() const;
        void ShootAt(const glm::ivec2& playerPos) const;

		BlueTankState* m_pState = nullptr;

        Transform* m_pTransform = nullptr;
        std::shared_ptr<Texture2D> m_pTankTexture = nullptr;
		std::shared_ptr<Texture2D> m_pTankGunTexture = nullptr;


        dae::GridNavigation* m_pNavigation = nullptr;
        GridMover* m_pMover = nullptr;
		std::vector<GridMover*> m_pPlayers;

		float m_ShootCooldown = 2.f; // seconds
        float m_ShootTimer = 0.0f;
    };
}

