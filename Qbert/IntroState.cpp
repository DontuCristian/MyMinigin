#include "IntroState.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "GameComponentsIncludes.h"
#include "MenuCommands.h"
#include "OpenLevelCommand.h"
#include "ChangeState.h"
#include "InputManager.h"
#include "Scoreboard.h"

void dae::IntroState::Enter() 
{
	auto& input = dae::InputManager::GetInstance();

	m_pScene = dae::SceneManager::GetInstance().GetScene("MainMenu");
	if (m_pScene == nullptr)
	{
		dae::SceneManager::GetInstance().CreateScene("MainMenu");
		m_pScene = dae::SceneManager::GetInstance().GetScene("MainMenu");
	}

	auto background = std::make_shared<dae::GameObject>();
	background->GetTransform()->SetLocalPosition(320, 120);
	background->AddComponent<dae::TextureRenderer>("logo.tga");
	m_pScene->Add(background);

	auto menuSelector = std::make_shared<dae::GameObject>();
	menuSelector->GetTransform()->SetLocalPosition(320,250);
	menuSelector->AddComponent<dae::MenuSelector>("Lingua.otf",10,"QBert_Selection.png");
	menuSelector->GetComponent<dae::MenuSelector>()->AddOption(dae::Option("Singleplayer",std::make_unique<dae::OpenLevelCommand>("Single")));
	menuSelector->GetComponent<dae::MenuSelector>()->AddOption(dae::Option("CO_OP", std::make_unique<dae::OpenLevelCommand>("Coop")));
	menuSelector->GetComponent<dae::MenuSelector>()->AddOption(dae::Option("PVP", std::make_unique<dae::OpenLevelCommand>("Pvp")));
	menuSelector->GetComponent<dae::MenuSelector>()->AddOption(dae::Option("ScoreBoard", std::make_unique<dae::ChangeState>(std::make_unique<Scoreboard>())));

	m_pScene->Add(menuSelector);


	input.AddAction("Next",SDL_SCANCODE_DOWN, TriggerEvent::Down,std::make_unique<MenuSelectionCommand>(*menuSelector.get(), dae::MenuSelectionType::Next));
	input.AddAction("Previous", SDL_SCANCODE_UP, TriggerEvent::Down, std::make_unique<MenuSelectionCommand>(*menuSelector.get(), dae::MenuSelectionType::Previous));
	input.AddAction("Confirm", SDL_SCANCODE_RETURN, TriggerEvent::Down, std::make_unique<MenuSelectionCommand>(*menuSelector.get(), dae::MenuSelectionType::Confirm));
	
	input.AddAction("Next", XINPUT_GAMEPAD_DPAD_DOWN, TriggerEvent::Down, std::make_unique<MenuSelectionCommand>(*menuSelector.get(), dae::MenuSelectionType::Next),1);
	input.AddAction("Previous", XINPUT_GAMEPAD_DPAD_UP, TriggerEvent::Down, std::make_unique<MenuSelectionCommand>(*menuSelector.get(), dae::MenuSelectionType::Previous),1);
	input.AddAction("Confirm", XINPUT_GAMEPAD_A, TriggerEvent::Down, std::make_unique<MenuSelectionCommand>(*menuSelector.get(), dae::MenuSelectionType::Confirm),1);
}

void dae::IntroState::Exit() 
{
	auto& input = dae::InputManager::GetInstance();
	input.RemoveAction("Next");
	input.RemoveAction("Previous");
	input.RemoveAction("Confirm");

	m_pScene->RemoveAll();
}

void dae::IntroState::Update() 
{
}