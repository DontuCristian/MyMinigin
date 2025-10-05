#include "GridMover.h"
#include "GameObject.h"
#include "Transform.h"
#include "Timer.h"

dae::GridMover::GridMover(GameObject& obj, GridNavigation* nav, int gx, int gy, int speed):
	BComponent(obj),
	m_GridX(gx),
	m_GridY(gy),
	m_Navigation(nav),
	m_MoveSpeed(speed)
{
	m_pTransform = obj.GetTransform();

	m_CurrentPos = m_Navigation->GetGridPosition(gx, gy);
}

void dae::GridMover::Update()
{
    if (m_Moving)
    {
        m_Progress += dae::Timer::GetInstance().GetDeltaTime() * m_MoveSpeed;
        if (m_Progress >= 1.0f)
        {
            m_Progress = 1.0f;
            m_Moving = false;
        }

        glm::vec2 newPos = glm::mix(m_CurrentPos, m_TargetPos, m_Progress);
        GetOwner()->GetTransform()->SetLocalPosition(newPos.x, newPos.y);

        if (!m_Moving)
        {
            m_CurrentPos = m_TargetPos;
        }
    }
}

void dae::GridMover::MoveBy(int dx, int dy)
{
    int nx = m_GridX + dx;
    int ny = m_GridY + dy;
    if (m_Navigation->IsWalkable(nx, ny) && !m_Moving)
    {
        m_GridX = nx;
        m_GridY = ny;
        m_TargetPos = m_Navigation->GetGridPosition(nx, ny);
        m_Moving = true;
        m_Progress = 0.0f;


        //Rotate the tank towards the next waypoint
        glm::ivec2 dir{ dx,dy };
        m_pTransform->SetRotation(static_cast<float>(glm::degrees(atan2(dir.y, dir.x))));
    }
}

void dae::GridMover::MoveTo(int gx, int gy)
{

    m_GridX = gx;
    m_GridY = gy;
    m_TargetPos = m_Navigation->GetGridPosition(gx, gy);
    m_CurrentPos = m_Navigation->GetGridPosition(gx, gy);
    m_Moving = false;
    m_Progress = 1.0f;

    GetOwner()->GetTransform()->SetLocalPosition(m_TargetPos.x, m_TargetPos.y);
}
