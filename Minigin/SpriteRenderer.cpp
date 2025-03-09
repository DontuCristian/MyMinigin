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
	m_FrameData = std::make_unique<FrameData>();
}

void dae::SpriteRenderer::Update()
{
	const int columns{m_Texture->GetSize().x/ m_FrameData->FrameWidth};

	//Update the column index
	const int colIdx = m_FrameData->CurrentFrame % columns;

	//Updating the SourceRect
	m_SourceRect = SDL_Rect{ static_cast<int>(colIdx * m_FrameData->FrameWidth),
							 0,
							 static_cast<int>(m_FrameData->FrameWidth),
							 static_cast<int>(m_FrameData->FrameHeight) };

	//Time counter to control the framerate
	m_FrameData->AccumulatedTime += Timer::GetInstance().GetDeltaTime();

	if (m_FrameData->AccumulatedTime > m_FrameData->FrameDelay)
	{
		++m_FrameData->CurrentFrame;
		if (m_FrameData->CurrentFrame >= columns)
		{
			m_FrameData->CurrentFrame = 0;
		}
		m_FrameData->AccumulatedTime -= m_FrameData->FrameDelay;
	}
}

void dae::SpriteRenderer::Render() const
{
	const auto pos = BComponent::GetOwner()->GetTransform()->GetWorldPosition();

	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x - m_FrameData->FrameWidth / 2, pos.y - m_FrameData->FrameHeight / 2,m_SourceRect);
}

void dae::SpriteRenderer::SetSpriteSheet(const std::string& path, int rows, int columns)
{
	auto texture = ResourceManager::GetInstance().LoadTexture(path);
	m_Texture = std::move(texture);

	m_FrameData->FrameWidth = m_Texture->GetSize().x / columns;
	m_FrameData->FrameHeight = m_Texture->GetSize().y / rows;
}
