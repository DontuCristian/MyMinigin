#pragma once
#include "Command.h"
namespace dae
{
    class IncreaseScoreCommand : public CommandObject
    {
	public:

		IncreaseScoreCommand(GameObject& obj, int score);
		~IncreaseScoreCommand() = default;

		IncreaseScoreCommand(const IncreaseScoreCommand& other) = delete;
		IncreaseScoreCommand(IncreaseScoreCommand&& other) = delete;
		IncreaseScoreCommand& operator=(const IncreaseScoreCommand& other) = delete;
		IncreaseScoreCommand& operator=(IncreaseScoreCommand&& other) = delete;

		void Execute() override;
	private:
		int m_ScoreAmmount{ 50 };
    };
}

