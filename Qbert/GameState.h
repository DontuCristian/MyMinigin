#pragma once

namespace dae
{

	class GameObject;

	class GameState
	{
	public:
		GameState() = default;
		virtual ~GameState() = default;

		// Pure virtual functions to be implemented by derived classes
		virtual void Enter(GameObject&) =0;
		virtual GameState* Update() = 0;
		virtual void Render() const =0;
		virtual void Exit() =0;
	};

}