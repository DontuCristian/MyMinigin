#pragma once

namespace dae
{
	class GameState
	{
	public:
		GameState() = default;
		virtual ~GameState() = default;

		// Pure virtual functions to be implemented by derived classes
		virtual void Enter() =0;
		virtual void Update() = 0;
		virtual void Render() const =0;
		virtual void Exit() =0;
	};

}