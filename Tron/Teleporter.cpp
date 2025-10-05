#include "Teleporter.h"
#include "GameObject.h"
#include "PhysicsComponents.h"
#include "Collisions.h"
#include "Transform.h"
#include "GridMover.h"
#include "BlueTank.h"
#include "ServiceLocator.h"
#include "SoundService.h"

dae::Teleporter::Teleporter(GameObject& obj, const std::vector<glm::ivec2>& destinations):
	BComponent(obj)
{

	obj.AddComponent<physics::Collider>(12.f, 12.f, glm::vec2{ -6.f,-6.f }, true);
	obj.GetComponent<physics::Collider>()->SetTag(make_sdbm_hash("Teleporter"));
	obj.GetComponent<physics::Collider>()->SetTriggerCallback(
		std::bind(&Teleporter::OnTrigger, this, std::placeholders::_1, std::placeholders::_2));

	m_Destinations = destinations;
}

void dae::Teleporter::Update()
{
}

void dae::Teleporter::Render() const
{
}

void dae::Teleporter::OnTrigger(const physics::Collider* other, const physics::CollisionPoints&)
{
	if (other->CompareTag("Player") || other->CompareTag("Enemy"))
	{
		ServiceLocator::GetSoundService().PlaySound("../Data/Tron/Sounds/Shoot.mp3", 0, 0.5f, false);

		//Pick a random destination from the list
		if (!m_Destinations.empty())
		{
			int randomIndex = rand() % m_Destinations.size();
			const auto& destination = m_Destinations[randomIndex];

			if(other->HasOwnerComponent<GridMover>() && other->CompareTag("Player"))
			{
				other->GetOwnerComponent<GridMover>()->MoveTo(destination.x, destination.y);
			}
			else if(other->HasOwnerComponent<BlueTank>() && other->CompareTag("Enemy"))
			{
				other->GetOwnerComponent<BlueTank>()->MoveTo(destination.x, destination.y);
			}
		}
		else
		{
			std::cerr << "No destinations available for teleportation!" << std::endl;
		}
	}
}