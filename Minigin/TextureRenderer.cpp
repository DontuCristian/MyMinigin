#include "TextureRenderer.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "GameObject.h"

dae::TextureRenderer::TextureRenderer(GameObject& owner, const std::string& path):
	BComponent(owner),
	m_Texture(nullptr)
{
	m_Transform = BComponent::GetOwner()->GetTransform();

	auto texture = ResourceManager::GetInstance().LoadTexture(path);
	m_Texture = std::move(texture);
}

void dae::TextureRenderer::Update()
{
}

void dae::TextureRenderer::Render() const
{
	const auto& size = glm::vec2{ m_Texture->GetSize().x,m_Texture->GetSize().y };
	const auto& pos = m_Transform->GetWorldPosition() - size/2.f;
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}

void dae::TextureRenderer::SetTexture( const std::string& path )
{
	auto texture = ResourceManager::GetInstance().LoadTexture(path);
	m_Texture = std::move(texture);
}