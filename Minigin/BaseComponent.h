#pragma once

namespace dae
{
	class GameObject;

	class BComponent
	{
	public:
		BComponent(GameObject* obj) { obj; };
		virtual ~BComponent() = default;


		BComponent(const BComponent& other) = default;
		BComponent(BComponent&& other) noexcept = default;
		BComponent& operator=(const BComponent& other) = default;
		BComponent& operator=(BComponent&& other)	noexcept = default;


		virtual void Update() {};
		virtual void FixedUpdate() {};

		virtual void Render() const { };
	protected:
	};
}

