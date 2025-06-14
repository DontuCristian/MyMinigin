#pragma once
#include <Command.h>
#include <string>

namespace dae
{
    class OpenLevelCommand : public Command
    {
        public:
        OpenLevelCommand(const std::string& mode)
            : m_LevelMode(mode) {}

        void Execute() override;

        private:
			std::string m_LevelMode; // The name of the state to open

    };
}

