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
	struct Option 
	{
		Option(const std::string& text, std::unique_ptr<Command>&& command)
			: Text(text), Command(std::move(command)) {
		}

		std::string Text;
		std::unique_ptr<Command> Command;
	};

	class Texture2D;

    class MenuSelector : public BComponent
    {	
	public:
		MenuSelector(GameObject& obj, const std::string& fontPath, int spacing, const std::string& texturePath);

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
		std::shared_ptr<dae::Texture2D> m_pSelectionQbertTexture;

		int m_CurrentOption{};
		int m_Spacing{};
		//int m_widestWidth{};
    };
}

