#include "EndState.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "InputNameCommand.h"
#include "OpenLevelCommand.h"
#include "SaveScoreCommand.h"
#include "InputManager.h"
#include "TextRenderer.h"
#include "TextInput.h"
#include "ChangeState.h"
#include "IntroState.h"
#include "ServiceLocator.h"
#include "SoundService.h"


dae::EndState::EndState(int score):
	m_Score{score}
{
}

void dae::EndState::Enter()
{
	dae::ServiceLocator::GetSoundService().PlaySound("../Data/Tron/Sounds/FailMusic.mp3", 0, 0.5f, false);

	auto& input = dae::InputManager::GetInstance();

	m_pScene = &dae::SceneManager::GetInstance().GetScene("Game");

	auto score = std::make_unique<dae::GameObject>();
	score->AddComponent<dae::TextRenderer>();
	score->GetComponent<dae::TextRenderer>()->SetFont("Lingua.otf",70);
	score->GetComponent<dae::TextRenderer>()->SetText("Score: " + std::to_string(m_Score));


	auto instructions = std::make_unique<dae::GameObject>();
	instructions->GetTransform()->SetLocalPosition(160, 400);
	instructions->AddComponent<dae::TextRenderer>();
	instructions->GetComponent<dae::TextRenderer>()->SetFont("Lingua.otf", 25);
	instructions->GetComponent<dae::TextRenderer>()->SetText("Press A or Enter to save the score");


	auto textInput = std::make_unique<dae::GameObject>();
	textInput->GetTransform()->SetLocalPosition(230,200);
	textInput->AddComponent<dae::TextInput>("Lingua.otf", 75, "Tron/Sprites/TronArrow.png");


	input.AddAction("Up", SDL_SCANCODE_UP, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput.get(), dae::InputTextAction::Up));
	input.AddAction("Down", SDL_SCANCODE_DOWN, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput.get(), dae::InputTextAction::Down));
	input.AddAction("Left", SDL_SCANCODE_LEFT, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput.get(), dae::InputTextAction::Left));
	input.AddAction("Right", SDL_SCANCODE_RIGHT, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput.get(), dae::InputTextAction::Right));
	input.AddAction("SaveScore", SDL_SCANCODE_RETURN, TriggerEvent::Down, std::make_unique<SaveScoreCommand>(*textInput.get(), *textInput.get(), m_Score, m_Score, "Single"));
	
	input.AddAction("Up", XINPUT_GAMEPAD_DPAD_UP, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput.get(), dae::InputTextAction::Up),0);
	input.AddAction("Down", XINPUT_GAMEPAD_DPAD_DOWN, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput.get(), dae::InputTextAction::Down),0);
	input.AddAction("Left", XINPUT_GAMEPAD_DPAD_LEFT, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput.get(), dae::InputTextAction::Left),0);
	input.AddAction("Right", XINPUT_GAMEPAD_DPAD_RIGHT, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput.get(), dae::InputTextAction::Right),0);
	input.AddAction("SaveScore", XINPUT_GAMEPAD_A, TriggerEvent::Down, std::make_unique<SaveScoreCommand>(*textInput.get(), *textInput.get(), m_Score, m_Score, "Single"), 0);

	m_pScene->Add(std::move(textInput));
	m_pScene->Add(std::move(instructions));
	m_pScene->Add(std::move(score));
}

void dae::EndState::Update()
{

}

void dae::EndState::Render() const
{

}

void dae::EndState::Exit()
{
	
	auto& input = InputManager::GetInstance();

	input.RemoveAction("Up");
	input.RemoveAction("Down");
	input.RemoveAction("Left");
	input.RemoveAction("Right");
	input.RemoveAction("SaveScore");

	m_pScene->RemoveAll();


}