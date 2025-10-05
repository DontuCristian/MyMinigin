#include "EndState2Players.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "InputNameCommand.h"
#include "OpenLevelCommand.h"
#include "SaveScoreCommand.h"
#include "InputManager.h"
#include "TextRenderer.h"
#include "TextInput.h"
#include "Scene.h"
#include "ChangeState.h"
#include "IntroState.h"
#include "ServiceLocator.h"
#include "SoundService.h"

dae::EndState2Players::EndState2Players(const std::vector<int>& playerScores, const std::string& mode):
	m_Scores(playerScores),
	m_Mode(mode)
{
}

void dae::EndState2Players::Enter()
{
	dae::ServiceLocator::GetSoundService().PlaySound("../Data/Tron/Sounds/FailMusic.mp3", 0, 0.5f, false);

	auto& input = dae::InputManager::GetInstance();

	m_pScene = &dae::SceneManager::GetInstance().GetScene("Game");

	auto instructions = std::make_unique<dae::GameObject>();
	instructions->GetTransform()->SetLocalPosition(160, 400);
	instructions->AddComponent<dae::TextRenderer>();
	instructions->GetComponent<dae::TextRenderer>()->SetFont("Lingua.otf", 25);
	instructions->GetComponent<dae::TextRenderer>()->SetText("Press A or Enter to save the score");

	auto score = std::make_unique<dae::GameObject>();
	score->GetTransform()->SetLocalPosition(100, 100);
	score->AddComponent<dae::TextRenderer>();
	score->GetComponent<dae::TextRenderer>()->SetFont("Lingua.otf", 50);
	score->GetComponent<dae::TextRenderer>()->SetText("Score: " + std::to_string(m_Scores[0]));


	auto textInput = std::make_unique<dae::GameObject>();
	textInput->GetTransform()->SetLocalPosition(100, 200);
	textInput->AddComponent<dae::TextInput>("Lingua.otf", 75, "Tron/Sprites/TronArrow.png");

	input.AddAction("Up0", SDL_SCANCODE_UP, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput.get(), dae::InputTextAction::Up));
	input.AddAction("Down0", SDL_SCANCODE_DOWN, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput.get(), dae::InputTextAction::Down));
	input.AddAction("Left0", SDL_SCANCODE_LEFT, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput.get(), dae::InputTextAction::Left));
	input.AddAction("Right0", SDL_SCANCODE_RIGHT, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput.get(), dae::InputTextAction::Right));

	m_pScene->Add(std::move(instructions));
	m_pScene->Add(std::move(score));

	score = std::make_unique<dae::GameObject>();
	score->AddComponent<dae::TextRenderer>();
	score->GetTransform()->SetLocalPosition(400, 100);
	score->GetComponent<dae::TextRenderer>()->SetFont("Lingua.otf", 50);
	score->GetComponent<dae::TextRenderer>()->SetText("Score: " + std::to_string(m_Scores[1]));


	auto textInput1 = std::make_unique<dae::GameObject>();
	textInput1->GetTransform()->SetLocalPosition(400, 200);
	textInput1->AddComponent<dae::TextInput>("Lingua.otf", 75, "Tron/Sprites/TronArrow.png");

	input.AddAction("Up1", SDL_SCANCODE_W, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput1.get(), dae::InputTextAction::Up));
	input.AddAction("Down1", SDL_SCANCODE_S, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput1.get(), dae::InputTextAction::Down));
	input.AddAction("Left1", SDL_SCANCODE_A, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput1.get(), dae::InputTextAction::Left));
	input.AddAction("Right1", SDL_SCANCODE_D, TriggerEvent::Down, std::make_unique<InputNameCommand>(*textInput1.get(), dae::InputTextAction::Right));

	input.AddAction("SaveScore", SDL_SCANCODE_RETURN, TriggerEvent::Down, std::make_unique<SaveScoreCommand>(*textInput.get(), *textInput1.get(), m_Scores[0], m_Scores[1], m_Mode));


	m_pScene->Add(std::move(textInput));
	m_pScene->Add(std::move(textInput1));
	m_pScene->Add(std::move(score));
}

void dae::EndState2Players::Update()
{
}

void dae::EndState2Players::Render() const
{
}

void dae::EndState2Players::Exit()
{
	auto& input = InputManager::GetInstance();

	input.RemoveAction("Up0");
	input.RemoveAction("Down0");
	input.RemoveAction("Left0");
	input.RemoveAction("Right0");
	input.RemoveAction("SaveScore");

	input.RemoveAction("Up1");
	input.RemoveAction("Down1");
	input.RemoveAction("Left1");
	input.RemoveAction("Right1");

	m_pScene->RemoveAll();
}