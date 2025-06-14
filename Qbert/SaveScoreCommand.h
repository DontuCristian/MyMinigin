#pragma once
#include <Command.h>
#include "TextInput.h"
namespace dae
{
    class SaveScoreCommand : public CommandObject
    {
    public:
        SaveScoreCommand(GameObject& obj, int score);
        virtual ~SaveScoreCommand() = default;

        virtual void Execute() override;
    private:

        void SaveScore(const std::string& name, int score, const std::string& path);

        std::string m_Name{};
        int m_Score{};

        TextInput* m_TextInput{};

    };
}

