#include "MenuSelector.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "GameObject.h"
#include <SDL_pixels.h>
#include <SDL_ttf.h>
#include <stdexcept>
#include <glm.hpp>

#include "ResourceManager.h"
#include "Transform.h"

dae::MenuSelector::MenuSelector(GameObject& obj, const std::string& fontPath, int spacing, const std::string& texturePath):
	BComponent(obj),
	m_Spacing(spacing)
{
	auto texture = ResourceManager::GetInstance().LoadTexture(texturePath); // Use texturePath instead of undefined "path"
	m_pSelectionQbertTexture = std::move(texture);

	auto font = ResourceManager::GetInstance().LoadFont(fontPath, 24); // Use fontPath instead of undefined "font"
	m_pFont = std::move(font);
}

void dae::MenuSelector::AddOption(Option&& option)
{
	// Add command to the list
	m_OptionCommands.emplace_back(std::move(option.Command));

	// Get the texture and compute which texture is the widest
	auto texture{ CreateTextureFromText(option.Text) };
	m_OptionTextures.emplace_back(texture);
}

void dae::MenuSelector::NextOption()
{
	m_CurrentOption = (m_CurrentOption + 1) % static_cast<int>(m_OptionTextures.size());
}

void dae::MenuSelector::PreviousOption()
{
	m_CurrentOption = m_CurrentOption - 1 < 0 ? static_cast<int>(m_OptionCommands.size()) - 1 : m_CurrentOption - 1;
}

void dae::MenuSelector::ConfirmOption()
{
	if (m_CurrentOption < m_OptionCommands.size())
	{
		m_OptionCommands[m_CurrentOption]->Execute();
	}
}

void dae::MenuSelector::Render() const
{
    glm::vec2 position{ GetOwner()->GetTransform()->GetWorldPosition() };

	for (size_t i = 0; i < m_OptionTextures.size(); ++i)
	{
		const auto& texture = m_OptionTextures[i];
		const auto size = texture->GetSize();

		// Calculate horizontal centering offset for the text
		const float xOffset = static_cast<float>(size.x) / 2.0f;

		// Draw the selection arrow if this is the current option
		if (static_cast<int>(i) == m_CurrentOption && m_pSelectionQbertTexture) 
		{
			const float arrowX = position.x - size.x;
			Renderer::GetInstance().RenderTexture(*m_pSelectionQbertTexture.get(), arrowX, position.y, static_cast<float>(size.y), static_cast<float>(size.y));
		}

		// Draw the option text centered
		Renderer::GetInstance().RenderTexture(*texture.get(), position.x - xOffset, position.y);
		position.y += size.y + m_Spacing;
	}
}

std::shared_ptr<dae::Texture2D> dae::MenuSelector::CreateTextureFromText(const std::string& text)
{
	const SDL_Color color = { static_cast<uint8_t>(255),static_cast<uint8_t>(255),static_cast<uint8_t>(255),static_cast<uint8_t>(255) };
	const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), text.c_str(), color);

	if (surf == nullptr) {
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}

	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr) {
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}

	SDL_FreeSurface(surf);
	return std::make_shared<Texture2D>(texture);
}