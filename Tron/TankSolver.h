#pragma once
#include <Collisions.h>

namespace dae::physics
{
    class TankSolver : public Solver
    {
    public:
        void SolveCollision(const Collision& collision) override;
    };
}

