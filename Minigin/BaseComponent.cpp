#include "BaseComponent.h"
#include "Transform.h"

dae::Transform* dae::BComponent::GetOwnerTransform() const
{
		return m_pOwner->GetTransform();
}