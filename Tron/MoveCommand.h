#pragma once
#include <Command.h>
#include <glm.hpp>

namespace dae
{
	class GridMover;
	class Transform;

	class MoveCommand : public CommandObject
	{
		public:
			MoveCommand(GameObject& obj, int dx, int dy);
			~MoveCommand() override = default;

			void Execute() override;
		private:
			
			GridMover* m_pGridMover{};
			Transform* m_pTransform{};

			int m_Dx, m_Dy;
	};
}

