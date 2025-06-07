#pragma once

namespace dae
{

	class GameState
	{

	public:
		GameState() = default;
		GameState(const GameState&) = delete;
		GameState(GameState&&) = delete;
		GameState& operator=(const GameState&) = delete;
		GameState& operator=(GameState&&) = delete;
		virtual ~GameState() = default;

		// Pure virtual functions to be implemented by derived classes
		virtual void Enter() = 0;
		virtual void Update() = 0;
		virtual void Render() = 0;
		virtual void Exit() = 0;
	};

}