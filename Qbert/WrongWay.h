#pragma once
#include "BaseComponent.h"
#include "AIMoveCommand.h"
#include <memory>

namespace dae
{
    class WrongWay :public BComponent
    {
    public:
        WrongWay(GameObject& obj, float moveInterval = 2.f);
        ~WrongWay() = default;

        void Update() override;
        void Render() const override {};
    private:

        std::unique_ptr<AIMoveCommand> m_pMoveRightCommand;
        std::unique_ptr<AIMoveCommand> m_pMoveLeftCommand;

        float m_MoveInterval{ 2.f };
		float m_MoveTimer{ 2.f };
    };
}

