#pragma once
#include <Command.h>
#include "TextInput.h"
namespace dae
{
    class SaveScoreCommand : public Command
    {
    public:
        SaveScoreCommand(GameObject& obj1, GameObject& obj2, int score1, int score2, const std::string& mode);
        virtual ~SaveScoreCommand() = default;

        virtual void Execute() override;
    private:

        void SaveScore(const std::string& p1, const std::string& p2, int score1, int score2, const std::string& mode, const std::string& path);

        std::string m_Name{};
        int m_Score1{};
		int m_Score2{};
		std::string m_Mode{};

        TextInput* m_pPlayer1{};
        TextInput* m_pPlayer2{};

    };
}

