#pragma once  
#include <functional>  
#include "GameActor.h"  
#include "GameObject.h"  

namespace dae
{
	class Command
	{
	public:
		Command() = default;
		virtual ~Command() = default;
		void Execute()
		{
			if(m_Command)
				m_Command();
		}

		void SetCommand(std::function<void()> command)
		{
			m_Command = command;
		}
	private:
		std::function<void()> m_Command{};
	};
}
