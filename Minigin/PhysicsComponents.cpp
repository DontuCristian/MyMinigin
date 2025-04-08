#include "PhysicsComponents.h"
#include "Transform.h"
#include "GameObject.h"

dae::physics::RigidBody::RigidBody(dae::GameObject& obj) :
	BComponent(obj),
	Gravity{ 0,0 },
	Force{ 0,0 },
	Velocity{ 0,0 },
	Mass{ 1 }
{
	pTransform = obj.GetTransform();
}

dae::physics::Collider::Collider(dae::GameObject& obj) :
	BComponent(obj),
	Offset{ 0,0 },
	Width{ 0 },
	Height{ 0 }
{
	pTransform = obj.GetTransform();
}