#pragma once
#include <Command.h>

namespace dae
{
    class Transform;
    
    class ChangeShootingAngle : public CommandObject
    {
    public:
        ChangeShootingAngle(GameObject& obj, float angle);
        ~ChangeShootingAngle() override = default;
    
        void Execute() override;
    private:
    
        Transform* m_pTransform{ nullptr };

		float m_Angle{ 0.f }; // Angle in degrees, clockwise
    };
}

