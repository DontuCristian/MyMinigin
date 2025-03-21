#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "MoveCommand.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	auto& input = dae::InputManager::GetInstance();

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextureRenderer>();
	go->GetComponent<dae::TextureRenderer>()->SetTexture("background.tga");
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextureRenderer>();
	go->GetComponent<dae::TextureRenderer>()->SetTexture("logo.tga");
	go->GetTransform()->SetLocalPosition(216, 180, 0);
	scene.Add(go);

	auto to = std::make_shared<dae::GameObject>();
	to->AddComponent<dae::TextRenderer>();
	to->GetComponent<dae::TextRenderer>()->SetFont("Lingua.otf", 36);
	to->GetComponent<dae::TextRenderer>()->SetText("Programming 4 Assignment");
	to->GetTransform()->SetLocalPosition(120, 20, 0);
	scene.Add(to);

	to = std::make_shared<dae::GameObject>();
	to->AddComponent<dae::FPSRenderer>();
	to->GetComponent<dae::TextRenderer>()->SetFont("Lingua.otf", 25);
	to->GetTransform()->SetLocalPosition(20, 30, 0);
	scene.Add(to);

	auto qBert = std::make_shared<dae::GameObject>();
	qBert->GetTransform()->SetLocalPosition(200, 100, 0);
	qBert->AddComponent<dae::SpriteRenderer>();
	qBert->GetComponent<dae::SpriteRenderer>()->SetSpriteSheet("Snake.png", 1, 8);
	qBert->GetComponent<dae::SpriteRenderer>()->SetFrameDelay(0.3f);
	scene.Add(qBert);

	input.GetInstance().AddAction("MoveLeft", SDL_SCANCODE_A, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*qBert.get(), glm::vec3{ -1,0,0 }, 200.f));
	input.GetInstance().AddAction("MoveRight", SDL_SCANCODE_D, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*qBert.get(), glm::vec3{ 1,0,0 }, 200.f));
	input.GetInstance().AddAction("MoveUp", SDL_SCANCODE_W, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*qBert.get(), glm::vec3{ 0,-1,0 }, 200.f));
	input.GetInstance().AddAction("MoveDown", SDL_SCANCODE_S, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*qBert.get(), glm::vec3{ 0,1,0 }, 200.f));



	auto snake = std::make_shared<dae::GameObject>();
	snake->GetTransform()->SetLocalPosition(100, 100, 0);
	snake->AddComponent<dae::SpriteRenderer>();
	snake->GetComponent<dae::SpriteRenderer>()->SetSpriteSheet("QBert1.png", 1, 8);
	snake->GetComponent<dae::SpriteRenderer>()->SetFrameDelay(0.3f);
	scene.Add(snake);

	input.GetInstance().AddAction("MoveLeft", XINPUT_GAMEPAD_DPAD_LEFT, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*snake.get(), glm::vec3{ -1,0,0 }, 200.f), 0);
	input.GetInstance().AddAction("MoveRight", XINPUT_GAMEPAD_DPAD_RIGHT, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*snake.get(), glm::vec3{ 1,0,0 }, 200.f), 0);
	input.GetInstance().AddAction("MoveUp", XINPUT_GAMEPAD_DPAD_UP, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*snake.get(), glm::vec3{ 0,-1,0 }, 200.f), 0);
	input.GetInstance().AddAction("MoveDown", XINPUT_GAMEPAD_DPAD_DOWN, dae::TriggerEvent::Pressed, std::make_unique<dae::MoveCommand>(*snake.get(), glm::vec3{ 0,1,0 }, 200.f), 0);


}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}
