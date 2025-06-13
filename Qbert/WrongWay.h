#pragma once
#include "BaseComponent.h"
#include "AIMoveCommand.h"
#include <memory>

namespace dae
{
    class WrongWay :public BComponent
    {
    public:
        WrongWay(GameObject& obj, const glm::vec2& upDir, const glm::vec2& downDir, float moveInterval = 2.f);
        ~WrongWay() = default;

        void Update() override;
        void Render() const override {};
    private:

        std::unique_ptr<AIMoveCommand> m_pMoveUpCommand;
        std::unique_ptr<AIMoveCommand> m_pMoveDownCommand;

        float m_MoveInterval{ 2.f };
		float m_MoveTimer{ 2.f };

        bool m_HasMovedOnce{ false };
    };
}

