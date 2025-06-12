#pragma once
namespace dae
{
    // Forward declaration
    class GameObject;
    class Transform;

    class CoilyState
    {
    public:
        CoilyState() = default;
        virtual ~CoilyState() = default;

        virtual void Enter(GameObject& obj, Transform* playerTransform) = 0;
        virtual CoilyState* Update() = 0;
        virtual void Exit() = 0;
    };
}
