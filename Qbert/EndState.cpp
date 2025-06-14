#include "EndState.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "GameComponentsIncludes.h"
#include "InputNameCommand.h"
#include "OpenLevelCommand.h"
#include "SaveScoreCommand.h"
#include "InputManager.h"

dae::EndState::EndState(int score):
	m_Score{score}
{
}

void dae::EndState::Enter()
{
	auto& input = dae::InputManager::GetInstance();

	m_pScene = dae::SceneManager::GetInstance().GetScene("EndScreen");
	if (m_pScene == nullptr)
	{
		dae::SceneManager::GetInstance().CreateScene("EndScreen");
		m_pScene = dae::SceneManager::GetInstance().GetScene("EndScreen");
	}

	auto score = std::make_shared<dae::GameObject>();
	score->AddComponent<dae::TextRenderer>();
	score->GetComponent<dae::TextRenderer>()->SetFont("Lingua.otf",70);
	score->GetComponent<dae::TextRenderer>()->SetText("Score: " + std::to_string(m_Score));
	m_pScene->Add(score);

	auto instructions = std::make_shared<dae::GameObject>();
	instructions->GetTransform()->SetLocalPosition(160, 400);
	instructions->AddComponent<dae::TextRenderer>();
	instructions->GetComponent<dae::TextRenderer>()->SetFont("Lingua.otf", 25);
	instructions->GetComponent<dae::TextRenderer>()->SetText("Press A or Enter to save the score");
	m_pScene->Add(instructions);

	auto textInput = std::make_shared<dae::GameObject>();
	textInput->GetTransform()->SetLocalPosition(230,200);
	textInput->AddComponent<dae::TextInput>("Lingua.otf", 75, "QBert_Selection.png");
	m_pScene->Add(textInput);


	input.AddAction("Up", SDL_SCANCODE_UP, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput.get(), dae::InputTextAction::Up));
	input.AddAction("Down", SDL_SCANCODE_DOWN, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput.get(), dae::InputTextAction::Down));
	input.AddAction("Left", SDL_SCANCODE_LEFT, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput.get(), dae::InputTextAction::Left));
	input.AddAction("Right", SDL_SCANCODE_RIGHT, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput.get(), dae::InputTextAction::Right));
	input.AddAction("SaveScore", SDL_SCANCODE_RETURN, TriggerEvent::Down, std::make_unique<SaveScoreCommand>(*textInput.get(), m_Score));

	input.AddAction("Up", XINPUT_GAMEPAD_DPAD_UP, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput.get(), dae::InputTextAction::Up),1);
	input.AddAction("Down", XINPUT_GAMEPAD_DPAD_DOWN, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput.get(), dae::InputTextAction::Down),1);
	input.AddAction("Left", XINPUT_GAMEPAD_DPAD_LEFT, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput.get(), dae::InputTextAction::Left),1);
	input.AddAction("Right", XINPUT_GAMEPAD_DPAD_RIGHT, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput.get(), dae::InputTextAction::Right),1);
	input.AddAction("SaveScore", XINPUT_GAMEPAD_A, TriggerEvent::Down, std::make_unique<SaveScoreCommand>(*textInput.get(), m_Score),1);
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