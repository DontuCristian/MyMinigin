#pragma once
#include <BaseComponent.h>
#include "AIMoveCommand.h"
#include <functional>
#include <memory>

namespace dae
{

	namespace physics
	{
		struct Collider;
		struct CollisionPoints;
	}

	class GameObject;

    class SamOrSlick :public BComponent
    {
    public:
        SamOrSlick(GameObject& obj);
        ~SamOrSlick() = default;

        void Update() override;
        void Render() const override {};


    private:
        std::function<void(const physics::Collider*, const physics::CollisionPoints&)> m_OnCollisionCallback{};
        void OnCollision(const physics::Collider* other, const physics::CollisionPoints& points);

        std::unique_ptr<AIMoveCommand> m_pMoveRightCommand;
        std::unique_ptr<AIMoveCommand> m_pMoveLeftCommand;

        float m_MoveInterval{ 2.f };
        float m_MoveTimer{ 2.f };

    };
}

