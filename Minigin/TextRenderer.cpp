#include "TextRenderer.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "GameObject.h"

dae::TextRenderer::TextRenderer(GameObject& owner) :
	BComponent(owner),
	m_TextTexture(nullptr),
	m_NeedsUpdate(false)
{
	m_Transform = BComponent::GetOwner()->GetTransform();
}

void dae::TextRenderer::Update()
{
	if (m_NeedsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_Color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_TextTexture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void dae::TextRenderer::Render() const
{
	if (m_TextTexture != nullptr)
	{
		const auto& pos = m_Transform->GetWorldPosition();
		Renderer::GetInstance().RenderTexture(*m_TextTexture, pos.x, pos.y);
	}
}

void dae::TextRenderer::SetText(const std::string& text)
{
	if (!text.empty())
	{
		m_Text = text;
		m_NeedsUpdate = true;
	}
}

void dae::TextRenderer::SetFont(const std::string& fontPath, int size)
{
	if (m_Text.empty())
	{
		m_Text = "No Text To Render";
	}

	auto font = ResourceManager::GetInstance().LoadFont(fontPath, static_cast<uint8_t>(size));
	m_Font = std::move(font);
	m_NeedsUpdate = true;
}
