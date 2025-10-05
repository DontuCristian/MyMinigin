#include "BlueTankChasing.h"
#include "GridNavigation.h"
#include "GridMover.h"
#include "Timer.h"
#include "BlueTankWandering.h"
#include <queue>
#include <map>

void dae::BlueTankChasing::Enter(GameObject& obj, const std::vector<GridMover*>& players, GridNavigation* nav)
{
	m_pMover = obj.GetComponent<GridMover>();
    m_pPlayers = players;
    m_pNavigation = nav;
}

dae::BlueTankState* dae::BlueTankChasing::Update()
{
    auto dt = dae::Timer::GetInstance().GetDeltaTime();
    m_PathTimer -= dt;

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

    if (!targetPlayer) return nullptr; // safety check

    auto playerPos = targetPlayer->GetGridPosition();


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

    if (abs(enemyPos.x - playerPos.x) >= 5 ||
        abs(enemyPos.y - playerPos.y) >= 5)
    {
        return new BlueTankWandering();
    }
    else
    {
        return nullptr;
    }
}

void dae::BlueTankChasing::Exit()
{
}

void dae::BlueTankChasing::RecalculatePath(const glm::ivec2& playerPos)
{
    m_CurrentPath = std::move(BFSPath(m_pNavigation, m_pMover->GetGridPosition(), playerPos));
    m_WaypointIndex = 1;
    m_LastKnownPlayerPos = playerPos;
    m_PathTimer = m_PathCooldown;
}

void dae::BlueTankChasing::MoveTo(int gx, int gy)
{
    m_pMover->MoveTo(gx, gy);
    RecalculatePath({ gx,gy });
}

std::vector<glm::ivec2> dae::BlueTankChasing::BFSPath(dae::GridNavigation* nav, glm::ivec2 start, glm::ivec2 goal)
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
