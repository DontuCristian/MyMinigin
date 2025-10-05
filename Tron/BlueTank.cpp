#include "BlueTank.h"
#include "GridNavigation.h"
#include "GridMover.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Timer.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Transform.h"
#include <queue>
#include <map>
#include "Bullet.h"
#include <EngineComponentsIncludes.h>
#include "BlueTankState.h"
#include "BlueTankWandering.h"
#include "BlueTankChasing.h"

dae::BlueTank::BlueTank(GameObject& obj, const std::vector<GridMover*>& players, GridNavigation* nav, const std::string& tankpath, const std::string& gunpath)
	: BComponent(obj), m_pNavigation(nav), m_pPlayers(players)
{
	m_pMover = obj.GetComponent<GridMover>();
	m_pTransform = obj.GetTransform();

    auto texture = ResourceManager::GetInstance().LoadTexture(tankpath);
    m_pTankTexture = std::move(texture);

	texture = ResourceManager::GetInstance().LoadTexture(gunpath);
	m_pTankGunTexture = std::move(texture);

	m_pState = new BlueTankWandering();
	m_pState->Enter(*GetOwner(), m_pPlayers, m_pNavigation);
}

void dae::BlueTank::Update()
{

    float dt = dae::Timer::GetInstance().GetDeltaTime();

    // Find closest player
    GridMover* targetPlayer = nullptr;
    int minDist = std::numeric_limits<int>::max();
    auto enemyPos = m_pMover->GetGridPosition();

    for (auto* player : m_pPlayers)
    {
        auto playerPos = player->GetGridPosition();
        int dist = abs(enemyPos.x - playerPos.x) + abs(enemyPos.y - playerPos.y);
        if (dist < minDist)
        {
            minDist = dist;
            targetPlayer = player;
        }
    }

    if (!targetPlayer) return; // safety check

    auto playerPos = targetPlayer->GetGridPosition();

    m_ShootTimer -= dt;

    if (m_ShootTimer <= 0.f)
    {
        if (CanSeePlayer() && !m_pMover->IsMoving())
        {
            ShootAt(playerPos);
            m_ShootTimer = m_ShootCooldown; // Reset shoot timer
        }
    }

    if (!CanSeePlayer())
    {
        auto* newState = m_pState->Update();

        if (newState != nullptr)
        {
            m_pState->Exit();
            m_pState = newState;
            m_pState->Enter(*GetOwner(), m_pPlayers, m_pNavigation);
        }
    }

}

void dae::BlueTank::Render() const
{

    const auto& size = glm::vec2{ m_pTankTexture->GetSize().x,m_pTankTexture->GetSize().y };
    const auto& pos = m_pTransform->GetWorldPosition() - size / 2.f;
    const auto& angle = m_pTransform->GetRotation();

    Renderer::GetInstance().RenderTexture(*m_pTankTexture, pos.x, pos.y, angle);

    const auto& gunSize = glm::vec2{ m_pTankGunTexture->GetSize().x,m_pTankGunTexture->GetSize().y };
    const auto& gunPos = m_pTransform->GetWorldPosition() - gunSize / 2.f;
    const auto& gunAngle = m_pTransform->GetRotation();

    Renderer::GetInstance().RenderTexture(*m_pTankGunTexture, gunPos.x, gunPos.y, gunAngle);

}


bool dae::BlueTank::CanSeePlayer() const
{
    // Find closest player
    GridMover* targetPlayer = nullptr;
    int minDist = std::numeric_limits<int>::max();
    auto enemyPos = m_pMover->GetGridPosition();

    for (auto* player : m_pPlayers)
    {
        auto playerPos = player->GetGridPosition();
        int dist = abs(enemyPos.x - playerPos.x) + abs(enemyPos.y - playerPos.y);
        if (dist < minDist)
        {
            minDist = dist;
            targetPlayer = player;
        }
    }

    if (!targetPlayer) return false; // safety check

    auto playerPos = targetPlayer->GetGridPosition();


    if (playerPos.y == enemyPos.y)
    {
        int startX = std::min(enemyPos.x, playerPos.x) + 1;
        int endX = std::max(enemyPos.x, playerPos.x);

        for (int x = startX; x < endX; ++x)
        {
            if (!m_pNavigation->IsWalkable(x, enemyPos.y))
            {
                return false; // Obstacle blocks shot
            }
        }

        return true;
    }

    if(playerPos.x == enemyPos.x)
    {
        int startY = std::min(enemyPos.y, playerPos.y) + 1;
        int endY = std::max(enemyPos.y, playerPos.y);
        for (int y = startY; y < endY; ++y)
        {
            if (!m_pNavigation->IsWalkable(enemyPos.x, y))
            {
                return false; // Obstacle blocks shot
            }
        }
        return true;
	}

    return false;


}

void dae::BlueTank::ShootAt(const glm::ivec2& playerPos) const
{
	auto enemyPos = m_pMover->GetGridPosition();

    auto dir = glm::normalize(static_cast<glm::vec2>(playerPos - enemyPos));

	auto& scene = dae::SceneManager::GetInstance().GetScene("Game");
	auto bullet = std::make_unique<GameObject>();
	bullet->GetTransform()->SetLocalPosition(m_pTransform->GetWorldPosition());
    bullet->AddComponent<Bullet>(dir, BulletOwner::Enemy);
    bullet->AddComponent<TextureRenderer>("Tron/Sprites/BulletNPC.png");

    scene.Add(std::move(bullet));

}

void dae::BlueTank::MoveTo(int gx, int gy)
{
	auto* wanderingState = dynamic_cast<BlueTankWandering*>(m_pState);

    if (wanderingState)
    {
        wanderingState->MoveTo(gx, gy);
        return;
    }

	auto* chasingState = dynamic_cast<BlueTankChasing*>(m_pState);

    if (chasingState)
    {
        chasingState->MoveTo(gx, gy);
        return;
    }

}


