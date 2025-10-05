#include "Recognizer.h"
#include "PhysicsComponents.h"
#include "GameObject.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "GridMover.h"
#include "GridNavigation.h"
#include "Timer.h"
#include "Texture2D.h"
#include "Health.h"
#include <queue>
#include <map>

dae::Recognizer::Recognizer(GameObject& obj, const std::vector<GridMover*>& players, GridNavigation* nav, const std::string& tankpath):
	BComponent(obj), m_pNavigation(nav), m_pPlayers(players)
{
	m_pMover = obj.GetComponent<GridMover>();
	m_pTransform = obj.GetTransform();

	auto texture = ResourceManager::GetInstance().LoadTexture(tankpath);
	m_pTankTexture = std::move(texture);

    obj.GetComponent<physics::Collider>()->SetTriggerCallback(
		std::bind(&Recognizer::OnTrigger, this, std::placeholders::_1, std::placeholders::_2));
}

void dae::Recognizer::Update()
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

    if (!targetPlayer) return; // safety check

    auto playerPos = targetPlayer->GetGridPosition();

    auto dt = dae::Timer::GetInstance().GetDeltaTime();
    m_PathTimer -= dt;

    // Only recalc if enough time passed or player moved grid cell
    if (m_PathTimer <= 0.0f && playerPos != m_LastKnownPlayerPos)
    {
        RecalculatePath(playerPos);
    }

    // If mover is idle and we still have waypoints, send it to next
    if (!m_pMover->IsMoving() && m_WaypointIndex < m_CurrentPath.size())
    {
        auto next = m_CurrentPath[m_WaypointIndex];

        m_pMover->MoveBy(next.x - m_pMover->GetGridPosition().x,
            next.y - m_pMover->GetGridPosition().y);
        m_WaypointIndex++;
    }
}

void dae::Recognizer::Render() const
{
    const auto& size = glm::vec2{ m_pTankTexture->GetSize().x,m_pTankTexture->GetSize().y };
    const auto& pos = m_pTransform->GetWorldPosition() - size / 2.f;
    const auto& angle = m_pTransform->GetRotation();

    Renderer::GetInstance().RenderTexture(*m_pTankTexture, pos.x, pos.y, angle);
}

void dae::Recognizer::RecalculatePath(const glm::ivec2& playerPos)
{
    m_CurrentPath = std::move(BFSPath(m_pNavigation, m_pMover->GetGridPosition(), playerPos));
    m_WaypointIndex = 1;
    m_LastKnownPlayerPos = playerPos;
    m_PathTimer = m_PathCooldown;
}

void dae::Recognizer::MoveTo(int gx, int gy)
{
    m_pMover->MoveTo(gx, gy);
    RecalculatePath({ gx,gy });
}

void dae::Recognizer::OnTrigger(const physics::Collider* other, const physics::CollisionPoints&)
{
    if (other->CompareTag("Player1") || other->CompareTag("Player2"))
    {
        other->GetOwnerComponent<Health>()->LoseLife();
    }
}

std::vector<glm::ivec2> dae::Recognizer::BFSPath(dae::GridNavigation* nav, glm::ivec2 start, glm::ivec2 goal)
{
    std::queue<glm::ivec2> q;
    std::map<glm::ivec2, glm::ivec2, bool(*)(const glm::ivec2&, const glm::ivec2&)> parent(
        [](const glm::ivec2& a, const glm::ivec2& b) { return a.y == b.y ? a.x < b.x : a.y < b.y; }
    );

    q.push(start);
    parent[start] = { -1,-1 };

    int dirs[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };

    while (!q.empty()) {
        auto cur = q.front(); q.pop();
        if (cur == goal) break;

        for (auto& d : dirs) {
            glm::ivec2 next = { cur.x + d[0], cur.y + d[1] };
            if (nav->IsWalkable(next.x, next.y) && !parent.count(next)) {
                parent[next] = cur;
                q.push(next);
            }
        }
    }

    std::vector<glm::ivec2> path;
    if (!parent.count(goal)) return path; // no path

    glm::ivec2 p = goal;
    while (p.x != -1) {
        path.push_back(p);
        p = parent[p];
    }
    std::reverse(path.begin(), path.end());
    return path;
}

