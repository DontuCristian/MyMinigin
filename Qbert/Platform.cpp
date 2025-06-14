#include "Platform.h"
#include "GameObject.h"
#include "Collisions.h"
#include "PhysicsComponents.h"
#include "Scene.h"

dae::Platform::Platform(GameObject& obj):
	BComponent(obj)
{
	m_OnCollisionCallback = std::bind(&Platform::OnCollision, this, std::placeholders::_1, std::placeholders::_2);
	obj.GetComponent<physics::Collider>()->SetCollisionCallback(m_OnCollisionCallback);

	m_LeftSide = GetOwnerTransform()->GetWorldPosition().x > 320 ? false : true;
}

void dae::Platform::Update()
{
	LiftPlayer();

#ifdef _DEBUG
	//std::cout << "PLatform position: "
	//	<< " X:" << GetOwnerTransform()->GetLocalPosition().x
	//	<< " Y:" << GetOwnerTransform()->GetLocalPosition().y
	//	<< std::endl;
	//
	//if (!GetOwner()->GetChildren().empty())
	//{
	//	std::cout << "Child force : "
	//		<< " X:" << m_pPlayer->GetTransform()->GetLocalPosition().x
	//		<< " Y:" << m_pPlayer->GetTransform()->GetLocalPosition().y
	//		<< std::endl;
	//}
#endif // _DEBUG


	if ((GetOwnerTransform()->GetWorldPosition().x > 320 && m_LeftSide) || (GetOwnerTransform()->GetWorldPosition().x < 320 && !m_LeftSide))
	{
		m_pPlayer->GetComponent<physics::RigidBody>()->IsKinematic = false;
		m_pPlayer->GetComponent<physics::RigidBody>()->Velocity = {0,0};
		m_pPlayer->GetComponent<physics::RigidBody>()->SetForce({ 0,0 },0);
		GetOwner()->FlagForDeletion();
	}
}

void dae::Platform::LiftPlayer()
{
	if (!GetOwner()->GetChildren().empty())
	{
		auto pos = GetOwnerTransform()->GetWorldPosition();
		pos.y -= 0.89f;
		pos.x += m_LeftSide ? 0.7f : -0.7f;

		GetOwnerTransform()->SetLocalPosition(pos);
	}
}
void dae::Platform::OnCollision(const physics::Collider* other, const physics::CollisionPoints& points)
{

	if (other->CompareTag("Player"))
	{
		if (points.Normal.y < 0 && points.Normal.x == 0)
		{
			m_pPlayer = other->GetOwner();

			m_pPlayer->SetParent(this->GetOwner(), true);
			m_pPlayer->GetTransform()->SetLocalPosition(0,-16);
			m_pPlayer->GetComponent<physics::RigidBody>()->IsKinematic = true;

			GetOwner()->GetComponent<dae::SpriteRenderer>()->PlayAnim();
		}

	}

}
