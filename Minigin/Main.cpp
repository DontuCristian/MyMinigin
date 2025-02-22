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

	//Empty object to orbit around kinda like a black hole
	auto emptyGo = std::make_shared<dae::GameObject>();
	emptyGo->GetTransform()->SetLocalPosition(100, 100, 0);
	scene.Add(emptyGo);

	auto planet = std::make_shared<dae::GameObject>();
	planet->SetParent(emptyGo.get(), true);
	planet->AddComponent<dae::SpriteRenderer>();
	planet->AddComponent<dae::Orbiter>();
	planet->GetComponent<dae::Orbiter>()->SetOrbitCenter(emptyGo->GetTransform()->GetWorldPosition());
	planet->GetComponent<dae::Orbiter>()->SetOrbitRadius(60);
	planet->GetComponent<dae::Orbiter>()->SetOrbitSpeed(180);
	planet->GetComponent<dae::SpriteRenderer>()->SetSpriteSheet("QBert1.png", 1, 8);
	planet->GetComponent<dae::SpriteRenderer>()->SetFrameDelay(0.3f);
	planet->GetComponent<dae::SpriteRenderer>()->PlayAnim();
	scene.Add(planet);

	auto moon = std::make_shared<dae::GameObject>();
	moon->SetParent(planet.get(), true);
	moon->AddComponent<dae::SpriteRenderer>();
	moon->AddComponent<dae::Orbiter>();
	moon->GetComponent<dae::Orbiter>()->SetOrbitCenter(planet->GetTransform()->GetWorldPosition());
	moon->GetComponent<dae::Orbiter>()->SetOrbitRadius(60);
	moon->GetComponent<dae::Orbiter>()->SetOrbitSpeed(360);
	moon->GetComponent<dae::SpriteRenderer>()->SetSpriteSheet("Snake.png", 1, 8);
	moon->GetComponent<dae::SpriteRenderer>()->SetFrameDelay(0.3f);
	moon->GetComponent<dae::SpriteRenderer>()->PlayAnim();
	scene.Add(moon);

}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}
