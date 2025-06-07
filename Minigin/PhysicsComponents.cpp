#include "PhysicsComponents.h"
#include "Transform.h"
#include "GameObject.h"
#include "PhysicsService.h"
#include "ServiceLocator.h"

dae::physics::RigidBody::RigidBody(dae::GameObject& obj) :
	BComponent(obj)
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

dae::physics::Collider::Collider(dae::GameObject& obj) :
	BComponent(obj),
	Offset{ 0,0 },
	Width{ 0 },
	Height{ 0 }
{
	pTransform = obj.GetTransform();
	//Remove the Collider from the physics service
	ServiceLocator::GetPhysicsService().AddCollider(this);
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

