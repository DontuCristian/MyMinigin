#pragma once
#include <string>
#include <memory>
#include <stdexcept>
#include <SDL_ttf.h>
#include <filesystem>

#include "BaseComponent.h"	
#include "Transform.h"

namespace dae
{
	class Font;
	class Texture2D;
	class GameObject;

	class TextRenderer final : public BComponent
	{
	public:
		TextRenderer(GameObject* obj);
		virtual ~TextRenderer() = default;

		TextRenderer(const TextRenderer& other) = delete;
		TextRenderer(TextRenderer&& other) = delete;
		TextRenderer& operator=(const TextRenderer& other) = delete;
		TextRenderer& operator=(TextRenderer&& other) = delete;

		void Update() override;
		void Render() const override;

		void SetText(const std::string& text);
		void SetFont(const std::string& fontPath, int size);

	private:

		Transform* m_Transform;

		std::shared_ptr<Font> m_Font;
		std::shared_ptr<Texture2D> m_TextTexture;
		std::string m_Text;

		//TO DO: make a warpper class for SDL_Color
		SDL_Color m_Color = { 255,255,255,255 };

		bool m_NeedsUpdate;

	};
}

