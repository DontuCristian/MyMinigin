#include "SpriteRenderer.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "Timer.h"
#include "Renderer.h"
#include "Transform.h"


dae::SpriteRenderer::SpriteRenderer(GameObject& owner):
	BComponent(owner)
{
}

void dae::SpriteRenderer::Update()
{
	//Update the column index
	m_ColIdx = m_CurrentFrame % m_Columns;

	//Updating the SourceRect
	m_SourceRect = SDL_Rect{ static_cast<int>(m_ColIdx * m_FrameWidth),
							 static_cast<int>(m_RowIdx * m_FrameHeight),
							 static_cast<int>(m_FrameWidth),
							 static_cast<int>(m_FrameHeight) };

	if (m_IsAnimated)
	{
		//Time counter to control the framerate
		m_AccumulatedTime += Timer::GetInstance().GetDeltaTime();

		if (m_AccumulatedTime > m_FrameDelay)
		{
			++m_CurrentFrame;
			if (m_CurrentFrame >= m_Columns)
			{
				m_CurrentFrame = 0;
			}
			m_AccumulatedTime -= m_FrameDelay;
		}
	}

}

void dae::SpriteRenderer::Render() const
{
	auto pos = BComponent::GetOwner()->GetTransform()->GetWorldPosition();

	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x - m_FrameWidth / 2, pos.y - m_FrameHeight / 2,m_SourceRect);
}

void dae::SpriteRenderer::SetSpriteSheet(const std::string& path, int rows, int columns)
{
	auto texture = ResourceManager::GetInstance().LoadTexture(path);
	m_Texture = std::move(texture);

	m_Rows = rows;
	m_Columns = columns;

	m_FrameWidth = m_Texture->GetSize().x / columns;
	m_FrameHeight = m_Texture->GetSize().y / rows;
}

void dae::SpriteRenderer::PlayAnim()
{
	m_IsAnimated = true;
}

void dae::SpriteRenderer::StopAnim()
{
	m_IsAnimated = false;
}

void dae::SpriteRenderer::ResetAnim()
{
	m_ColIdx = 0;
	m_CurrentFrame = 0;
}
