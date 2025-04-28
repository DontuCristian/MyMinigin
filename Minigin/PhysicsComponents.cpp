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