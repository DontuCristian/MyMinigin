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
#include "GameObject.h"
#include "Scene.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextureRenderer>();
	go->GetComponent<dae::TextureRenderer>()->SetTexture("background.tga");
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextureRenderer>();
	go->GetComponent<dae::TextureRenderer>()->SetTexture("logo.tga");
	go->GetComponent<dae::Transform>()->SetPosition(216, 180,0);
	scene.Add(go);

	auto to = std::make_shared<dae::GameObject>();
	to->AddComponent<dae::TextRenderer>();
	to->GetComponent<dae::TextRenderer>()->SetFont("Lingua.otf", 36);
	to->GetComponent<dae::TextRenderer>()->SetText("Programming 4 Assignment");
	to->GetComponent<dae::Transform>()->SetPosition(120, 20, 0);
	scene.Add(to);

	to = std::make_shared<dae::GameObject>();
	to->AddComponent<dae::TextRenderer>();
	to->AddComponent<dae::FPSRenderer>();
	to->GetComponent<dae::TextRenderer>()->SetFont("Lingua.otf", 25);
	to->GetComponent<dae::Transform>()->SetPosition(20, 30, 0);
	scene.Add(to);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}