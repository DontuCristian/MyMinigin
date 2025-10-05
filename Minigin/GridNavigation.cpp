#include "GridNavigation.h"
#include "Renderer.h"

dae::GridNavigation::GridNavigation(GameObject& obj, int width, int height):
	BComponent(obj)
{
	m_GridPoints.resize(static_cast<size_t>(height), std::vector<GridPoint>(static_cast<size_t>(width)));

}

void dae::GridNavigation::Update()
{
}

void dae::GridNavigation::Render() const
{
#ifdef _DEBUG

	for (int y{}; y < m_GridPoints.size(); ++y)
	{
		for (int x{}; x < m_GridPoints[0].size(); ++x)
		{
			if (m_GridPoints[y][x].IsWalkable)
			{
				SDL_Rect rect;
				rect.x = static_cast<int>(m_GridPoints[y][x].Position.x) - 8;
				rect.y = static_cast<int>(m_GridPoints[y][x].Position.y) - 8;
				rect.w = 16; // Width of the grid point
				rect.h = 16; // Height of the grid point
	
				SDL_Color color{ 0, 255, 0, 255 }; // Red color for non-walkable points
	
				// Render non-walkable grid point (e.g., draw a different color)
				Renderer::GetInstance().RenderRect(rect, color);
			}
			else
			{
				SDL_Rect rect;
				rect.x = static_cast<int>(m_GridPoints[y][x].Position.x) - 8;
				rect.y = static_cast<int>(m_GridPoints[y][x].Position.y) - 8;
				rect.w = 16; // Width of the grid point
				rect.h = 16; // Height of the grid point
	
				SDL_Color color{ 255, 0, 0, 255 }; // Red color for non-walkable points
	
				// Render non-walkable grid point (e.g., draw a different color)
				Renderer::GetInstance().RenderRect(rect, color);
			}
		}
	}

#endif // _DEBUG
}

void dae::GridNavigation::SetWalkable(int x, int y, bool walkable)
{

	if (x < 0 || x >= static_cast<int>(m_GridPoints[0].size()) ||
		y < 0 || y >= static_cast<int>(m_GridPoints.size()))
	{
		return; // Out of bounds
	}

	m_GridPoints[y][x].IsWalkable = walkable;
}

void dae::GridNavigation::SetPosition(int x, int y, const glm::vec2& position)
{
	if (x < 0 || x >= static_cast<int>(m_GridPoints[0].size()) ||
		y < 0 || y >= static_cast<int>(m_GridPoints.size()))
	{
		return; // Out of bounds
	}

	m_GridPoints[y][x].Position = position;
}



bool dae::GridNavigation::IsWalkable(int x, int y) const
{
	if (x < 0 || x >= static_cast<int>(m_GridPoints[0].size()) ||
		y < 0 || y >= static_cast<int>(m_GridPoints.size()))
	{
		return false; // Out of bounds
	}
	return m_GridPoints[y][x].IsWalkable;
}

glm::vec2 dae::GridNavigation::GetGridPosition(int x, int y) const
{
	if (x < 0 || x >= static_cast<int>(m_GridPoints[0].size()) ||
		y < 0 || y >= static_cast<int>(m_GridPoints.size()))
	{
		return glm::vec2{ -100, -100 }; // Out of bounds, return default position
	}
	return m_GridPoints[y][x].Position;
}

int dae::GridNavigation::GetGridWidth() const
{
	return static_cast<int>(m_GridPoints[0].size());
}

int dae::GridNavigation::GetGridHeight() const
{
	return static_cast<int>(m_GridPoints.size());
}
