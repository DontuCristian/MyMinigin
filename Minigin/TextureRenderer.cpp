#include "TextureRenderer.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "GameObject.h"

dae::TextureRenderer::TextureRenderer(GameObject& obj):
	BComponent(obj),
	m_Texture(nullptr)
{
	m_Transform = BComponent::GetOwner()->GetTransform();
}

void dae::TextureRenderer::Update()
{
}

void dae::TextureRenderer::Render() const
{
	const auto& pos = m_Transform->GetPosition();
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}

void dae::TextureRenderer::SetTexture( const std::string& path )
{
	auto texture = ResourceManager::GetInstance().LoadTexture(path);
	m_Texture = std::move(texture);
}
