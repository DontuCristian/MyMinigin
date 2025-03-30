#pragma once
#include "Command.h"

namespace dae
{
	class TakeDamageCommand final : public CommandObject
	{
	public:

		TakeDamageCommand(GameObject& obj, int dmg);
		~TakeDamageCommand() = default;

		TakeDamageCommand(const TakeDamageCommand& other) = delete;
		TakeDamageCommand(TakeDamageCommand&& other) = delete;
		TakeDamageCommand& operator=(const TakeDamageCommand& other) = delete;
		TakeDamageCommand& operator=(TakeDamageCommand&& other) = delete;

		void Execute() override;
	private:
		int m_DamageAmmount{10};
	};
}

