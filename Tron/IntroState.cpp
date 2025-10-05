#include "IntroState.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "MenuCommands.h"
#include "OpenLevelCommand.h"
#include "ChangeState.h"
#include "InputManager.h"
#include "MenuSelector.h"
#include "TextureRenderer.h"
#include "Transform.h"
#include "Scoreboard.h"
#include "MuteCommand.h"

void dae::IntroState::Enter() 
{
	auto& input = dae::InputManager::GetInstance();

	m_pScene = &dae::SceneManager::GetInstance().GetScene("Game");

	auto background = std::make_unique<dae::GameObject>();
	background->GetTransform()->SetLocalPosition(320, 120);
	background->AddComponent<dae::TextureRenderer>("logo.tga");

	auto menuSelector = std::make_unique<dae::GameObject>();
	menuSelector->GetTransform()->SetLocalPosition(320,250);
	menuSelector->AddComponent<dae::MenuSelector>("Lingua.otf",10, "Tron/Sprites/TronArrow.png");
	menuSelector->GetComponent<dae::MenuSelector>()->AddOption(dae::Option("Singleplayer",std::make_unique<dae::OpenLevelCommand>(0,"Single")));
	menuSelector->GetComponent<dae::MenuSelector>()->AddOption(dae::Option("CO_OP", std::make_unique<dae::OpenLevelCommand>(0,"Coop")));
	menuSelector->GetComponent<dae::MenuSelector>()->AddOption(dae::Option("PVP", std::make_unique<dae::OpenLevelCommand>(0,"Pvp")));
	menuSelector->GetComponent<dae::MenuSelector>()->AddOption(dae::Option("ScoreBoard", std::make_unique<dae::ChangeState>(std::make_unique<Scoreboard>())));



	input.AddAction("Next",SDL_SCANCODE_DOWN, TriggerEvent::Down,std::make_unique<MenuSelectionCommand>(*menuSelector.get(), dae::MenuSelectionType::Next));
	input.AddAction("Previous", SDL_SCANCODE_UP, TriggerEvent::Down, std::make_unique<MenuSelectionCommand>(*menuSelector.get(), dae::MenuSelectionType::Previous));
	input.AddAction("Confirm", SDL_SCANCODE_RETURN, TriggerEvent::Down, std::make_unique<MenuSelectionCommand>(*menuSelector.get(), dae::MenuSelectionType::Confirm));
	
	input.AddAction("Next", XINPUT_GAMEPAD_DPAD_DOWN, TriggerEvent::Down, std::make_unique<MenuSelectionCommand>(*menuSelector.get(), dae::MenuSelectionType::Next),1);
	input.AddAction("Previous", XINPUT_GAMEPAD_DPAD_UP, TriggerEvent::Down, std::make_unique<MenuSelectionCommand>(*menuSelector.get(), dae::MenuSelectionType::Previous),1);
	input.AddAction("Confirm", XINPUT_GAMEPAD_A, TriggerEvent::Down, std::make_unique<MenuSelectionCommand>(*menuSelector.get(), dae::MenuSelectionType::Confirm),1);

	input.AddAction("Mute", SDL_SCANCODE_F2, TriggerEvent::Down, std::make_unique<MuteCommand>());

	m_pScene->Add(std::move(menuSelector));
	m_pScene->Add(std::move(background));
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