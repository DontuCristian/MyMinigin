#pragma once  
#include <functional>  

namespace dae
{
	class GameObject;

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

	private:

		GameObject* m_pGameObject;
	};
}
