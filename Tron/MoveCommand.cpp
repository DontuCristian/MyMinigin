#include "MoveCommand.h"
#include "PhysicsComponents.h"
#include "ServiceLocator.h"
#include "TextureRenderer.h"
#include "Transform.h"
#include "GridMover.h"
#include "GameObject.h"
#include <iostream>

dae::MoveCommand::MoveCommand(GameObject& obj, int dx, int dy):
	CommandObject(obj),
	m_Dx(dx),
	m_Dy(dy)
{
	m_pGridMover = obj.GetComponent<GridMover>();
	m_pTransform = obj.GetTransform();
}

void dae::MoveCommand::Execute()
{
	std::cout << "GridMover MoveCommand: " << m_pGridMover << std::endl;

	if(m_pGridMover)
		m_pGridMover->MoveBy(m_Dx, m_Dy);
}