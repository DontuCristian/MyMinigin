#include "TextInput.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Transform.h"
#include "GameObject.h"
#include <SDL_pixels.h>
#include <SDL_ttf.h>
#include <stdexcept>
#include <glm.hpp>

void dae::TextInput::Up()
{
	int& value = m_CharacterValues[m_Index];
	value = (value + 1) % static_cast<int>(CHARACTERS.length());
	m_isDirty = true;
}

void dae::TextInput::Down()
{
	int& value = m_CharacterValues[m_Index];
	//Blocks it from going bellow 0
	value = (value - 1 + static_cast<int>(CHARACTERS.length())) % static_cast<int>(CHARACTERS.length());
	m_isDirty = true;
}

void dae::TextInput::Left()
{
	//This blocks it from going bellow 0
	m_Index = (m_Index - 1 + SIZE) % SIZE;
}

void dae::TextInput::Right()
{
	m_Index = (m_Index + 1) % SIZE;
}

dae::TextInput::TextInput(GameObject& obj, const std::string& fontPath, int characterSpacing, const std::string& pointerTexturePath):
	BComponent(obj),
	m_Spacing{characterSpacing}
{
	auto texture = ResourceManager::GetInstance().LoadTexture(pointerTexturePath);
	m_pPointerTexture = std::move(texture);

	auto font = ResourceManager::GetInstance().LoadFont(fontPath,100);
	m_pFont = std::move(font);

}

void dae::TextInput::Update()
{
	if (m_isDirty)
	{
		m_Result.clear();
		m_pCharacterTextures.clear();

		for (int c : m_CharacterValues)
		{
			char character{ CHARACTERS[c] };
			if (c != 0)
			{
				m_Result.push_back(character);
			}

			std::string singleChar{};
			singleChar.push_back(character);

			const SDL_Color color = { static_cast<uint8_t>(255),static_cast<uint8_t>(255),static_cast<uint8_t>(255),static_cast<uint8_t>(255) };
			const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), singleChar.c_str(), color);

			if (surf == nullptr) {
				throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
			}

			auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
			if (texture == nullptr) {
				throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
			}

			SDL_FreeSurface(surf);
			m_pCharacterTextures.push_back(std::make_shared<Texture2D>(texture));

		}

		m_isDirty = false;
	}
}

void dae::TextInput::Render() const
{
	const auto& pos{ GetOwnerTransform()->GetWorldPosition() };

	for (int idx{}; idx<m_pCharacterTextures.size();++idx)
	{
		const auto& texture{ m_pCharacterTextures[idx] };

		if (texture)
		{
			Renderer::GetInstance().RenderTexture(*texture, pos.x + idx * m_Spacing, pos.y);
		}
	}

	const int pointerYOffset = 70;
	Renderer::GetInstance().RenderTexture(*m_pPointerTexture, pos.x + m_Index * m_Spacing, pos.y + pointerYOffset);
}

