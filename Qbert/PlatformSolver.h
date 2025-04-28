#pragma once
#include "Collisions.h"

namespace dae::physics
{
	class PlatformSolver : public Solver
	{
	public:
		PlatformSolver() = default;
		~PlatformSolver() override = default;

		void SolveCollision(const Collision& collision) override;
	};
}