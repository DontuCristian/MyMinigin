#include "ChangeShootingAngle.h"
#include "GameObject.h"
#include "Transform.h"
#include "Timer.h"

dae::ChangeShootingAngle::ChangeShootingAngle(GameObject& obj, float angle):
	CommandObject(obj),
	m_Angle{ angle }
{
	m_pTransform = obj.GetTransform();
}

void dae::ChangeShootingAngle::Execute()
{

	float angle = m_pTransform->GetRotation();
	angle += m_Angle * Timer::GetInstance().GetDeltaTime();

	// Normalize the angle to be within [0, 360)
	if (angle >= 360.f)
		angle -= 360.f;
	else if (angle < 0.f)
		angle += 360.f;

	m_pTransform->SetRotation(angle);
}
