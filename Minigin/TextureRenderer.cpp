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
	m_Transform = owner.GetTransform();

	auto texture = ResourceManager::GetInstance().LoadTexture(path);
	m_Texture = std::move(texture);
}

dae::TextureRenderer::TextureRenderer(GameObject& owner, std::shared_ptr<Texture2D> texture):
	BComponent(owner),
	m_Texture(std::move(texture))
{
	m_Transform = owner.GetTransform();
}

dae::TextureRenderer::TextureRenderer(GameObject& owner):
	BComponent(owner)
{
	m_Transform = owner.GetTransform();
}

void dae::TextureRenderer::Update()
{
}

void dae::TextureRenderer::Render() const
{
	const auto& size = glm::vec2{ m_Texture->GetSize().x,m_Texture->GetSize().y };
	const auto& pos = m_Transform->GetWorldPosition() - size/2.f;
	const auto& angle = m_Transform->GetRotation();

	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, angle);
}

void dae::TextureRenderer::SetTexture( const std::string& path )
{
	auto texture = ResourceManager::GetInstance().LoadTexture(path);
	m_Texture = std::move(texture);
}
