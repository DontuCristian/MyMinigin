#include "ShootCommand.h"
#include "GameObject.h"
#include "PhysicsComponents.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Transform.h"
#include "TextureRenderer.h"
#include <memory>
#include "Bullet.h"
#include "ServiceLocator.h"
#include "SoundService.h"

dae::ShootCommand::ShootCommand(GameObject& obj, int idx):
	CommandObject(obj),
	m_PlayerIdx{ idx }
{

	m_pTransform = obj.GetTransform();
}

void dae::ShootCommand::Execute()
{
	ServiceLocator::GetSoundService().PlaySound("../Data/Tron/Sounds/Shoot.mp3", 0, 0.5f, false);

	auto& scene = dae::SceneManager::GetInstance().GetScene("Game");

	float angle = m_pTransform->GetRotation();
	// Convert angle to radians for bullet direction
	glm::vec2 direction = glm::vec2(cos(glm::radians(angle)), sin(glm::radians(angle)));

	auto bullet = std::make_unique<GameObject>();
	bullet->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetWorldPosition());
	bullet->AddComponent<Bullet>(direction,static_cast<BulletOwner>(m_PlayerIdx));
	bullet->AddComponent<TextureRenderer>("Tron/Sprites/BulletPlayer.png");

	scene.Add(std::move(bullet));
}
