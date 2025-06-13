#pragma once
#include <BaseComponent.h>
#include "Command.h"
#include "Font.h"
#include "Texture2D.h"
#include "Subject.h"
#include <string>
#include <memory>

namespace dae
{
    class MenuSelector : public BComponent
    {
		struct Option {
			std::string text;
			std::unique_ptr<Command> command;
		};

		MenuSelector(GameObject& pOwner, std::shared_ptr<Font> font,int spacing, std::shared_ptr<Texture2D> pSelectionArrowTexture);

		void AddOption(Option&& option);

		void NextOption();
		void PreviousOption();
		void ConfirmOption();

		void Update() override {};
		void Render() const override;

	private:
		std::shared_ptr<Texture2D> CreateTextureFromText(const std::string& text);

		std::shared_ptr<Font> m_pFont;

		std::vector<std::shared_ptr<Texture2D>> m_OptionTextures;
		std::vector<std::unique_ptr<Command>> m_OptionCommands;
		std::shared_ptr<Texture2D> m_pSelectionArrowTexture;

		int m_CurrentOption{};
		int m_Spacing{};
		//int m_widestWidth{};
    };
}

