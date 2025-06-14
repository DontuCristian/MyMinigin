#pragma once
#include <Command.h>
#include <string>

namespace dae
{
    class ChangeLevelCommand : public Command
    {
    public:
        ChangeLevelCommand(int index, const std::string& mode);

        void Execute() override;

    private:

        int m_Index{};
        std::string m_Mode{};

    };
}

