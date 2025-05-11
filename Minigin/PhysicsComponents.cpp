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
	glm::vec2 pos = pTransform->GetWorldPosition() + Offset;
	glm::vec2 size{ Width, Height };
	SDL_Rect rect{ static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.x), static_cast<int>(size.y) };
	Renderer::GetInstance().RenderRect(rect, SDL_Color{ 255,0,0,255 });
#endif

};
