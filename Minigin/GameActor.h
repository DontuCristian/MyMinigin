#pragma once
#include "GameObject.h"
#include "Command.h"
#include "InputManager.h"

namespace dae
{
	class GameActor : public GameObject
	{
	public:
		GameActor() = default;
		virtual ~GameActor() = default;

		GameActor(const GameActor& other) = delete;
		GameActor(GameActor&& other) = delete;
		GameActor& operator=(const GameActor& other) = delete;
		GameActor& operator=(GameActor&& other) = delete;

		virtual void Update() { GameObject::Update(); }
		virtual void FixedUpdate() { GameObject::FixedUpdate(); }
		virtual void Render() const { GameObject::Render(); }
	};
}

