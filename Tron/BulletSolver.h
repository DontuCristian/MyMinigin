#pragma once
#include <Collisions.h>


namespace dae::physics
{
    class BulletSolver : public Solver
    {
        public:
	    void SolveCollision(const Collision& collision) override;

    };
}

