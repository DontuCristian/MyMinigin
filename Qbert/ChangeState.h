#pragma once
#include <Command.h>
#include <memory>
#include "GameState.h"

namespace dae
{
    class ChangeState : public Command
    {
    public:
        ChangeState(std::unique_ptr<GameState> state);
        ~ChangeState() = default;

        void Execute() override;
    private:

        std::unique_ptr<GameState> m_State{};
    };
}

