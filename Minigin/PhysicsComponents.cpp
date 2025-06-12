#include "PhysicsComponents.h"
#include "Transform.h"
#include "GameObject.h"
#include "PhysicsService.h"
#include "ServiceLocator.h"

dae::physics::RigidBody::RigidBody(dae::GameObject& obj, bool isKin) :
	BComponent(obj),
	GravityScale{9.81f},
	Gravity{ 0,1 },
	IsKinematic{isKin}
{
	pTransform = obj.GetTransform();

	//Add the RigidBody to the physics service
	ServiceLocator::GetPhysicsService().AddRigidBody(this);
}

dae::physics::RigidBody::~RigidBody()
{

	//Remove the RigidBody from the physics service
	ServiceLocator::GetPhysicsService().RemoveRigidBody(this);
}

void dae::physics::RigidBody::AddForce(const glm::vec2& direction, float strength)
{
	Force += direction * strength;
}
void dae::physics::RigidBody::SetForce(const glm::vec2& direction, float strength)
{
	Force = direction * strength;
}

dae::physics::Collider::Collider(dae::GameObject& obj, float width, float height, glm::vec2 offset, bool isTrigger) :
	BComponent(obj),
	Offset{ offset },
	Width{ width },
	Height{ height },
	IsTrigger{ isTrigger }
{
	pTransform = obj.GetTransform();

	//Remove the Collider from the physics service
	ServiceLocator::GetPhysicsService().AddCollider(this);

	m_OnCollisionCallbacks.clear();
	m_OnTriggerCallbacks.clear();
}

dae::physics::Collider::~Collider()
{
	//Remove the Collider from the physics service
	ServiceLocator::GetPhysicsService().RemoveCollider(this);
}

void dae::physics::Collider::Render() const
{

#ifdef _DEBUG
	//Draw the collider
	glm::vec2 pos = pTransform->GetWorldPosition();

	SDL_Rect rect{ static_cast<int>(pos.x) + static_cast<int>(Offset.x), static_cast<int>(pos.y) + static_cast<int>(Offset.y), static_cast<int>(Width), static_cast<int>(Height) };

	if (IsColliding)
	{
		Renderer::GetInstance().RenderRect(rect, SDL_Color{ 0,255,0,255 });
	}
	else
	{
		Renderer::GetInstance().RenderRect(rect, SDL_Color{ 255,0,0,255 });
	}
#endif

}
bool dae::physics::Collider::CompareTag(const std::string& tag) const
{
	if(tag.empty())
		return false;

	auto hash = make_sdbm_hash_str(tag);

	return 	hash == m_Tag;
}

void dae::physics::Collider::SetTag(const std::string& tag)
{
	if (tag.empty())
		return;
	m_Tag = make_sdbm_hash_str(tag);
}

void dae::physics::Collider::OnCollision(const Collider* other, const CollisionPoints& points)
{
	if (other->IsTrigger == false)
	{
		for (const auto& onCollision : m_OnCollisionCallbacks)
		{
			if (onCollision)
			{
#ifdef _DEBUG
				//std::cout << "WasColliding: " << WasColliding << " IsColliding: " << IsColliding << std::endl;
				std::cout << "OnCollision called! " << std::endl;
#endif // _DEBUG
				onCollision(other, points);
			}
		}

	}
}

void dae::physics::Collider::OnTrigger(const Collider* other, const CollisionPoints& points)
{
	if (other->IsTrigger == true)
	{
		for (const auto& onTrigger : m_OnTriggerCallbacks)
		{
#ifdef _DEBUG
			//std::cout << "WasCollidingTrigger: " << WasCollidingTrigger << " IsCollidingTrigger: " << IsCollidingTrigger << std::endl;
#endif // _DEBUG


			if (onTrigger)
			{
#ifdef _DEBUG
				std::cout << "OnTrigger called! " << std::endl;
#endif // _DEBUG
				onTrigger(other, points);
			}
		}
	}
}

