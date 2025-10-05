#pragma once
#include <Command.h>
#include <string>

namespace dae
{
    class OpenLevelCommand : public Command
    {
        public:
        OpenLevelCommand(int index, const std::string& mode)
            : m_LevelMode(mode),
            m_LevelIndex(index){}

        void Execute() override;

        private:
			std::string m_LevelMode; // The name of the state to open
			int m_LevelIndex{ 0 };

    };
}

