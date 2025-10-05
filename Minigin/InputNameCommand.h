#pragma once
#include <Command.h>
namespace dae
{
    enum class InputTextAction
    {
        Up,
        Down,
        Left,
        Right
    };

    class InputNameCommand :public CommandObject
    {
    public:
        InputNameCommand(GameObject& obj, InputTextAction action);
        virtual ~InputNameCommand() = default;

        virtual void Execute() override;
    private:

        InputTextAction m_Action;
    };
}

