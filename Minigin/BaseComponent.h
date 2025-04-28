#pragma once

namespace dae
{
	class GameObject;

	class BComponent
	{
	public:
		virtual ~BComponent() = default;

		BComponent(const BComponent& other) = delete;
		BComponent(BComponent&& other) noexcept = delete;
		BComponent& operator=(const BComponent& other) = delete;
		BComponent& operator=(BComponent&& other)	noexcept = delete;


		virtual void Update() = 0;
		virtual void Render() const = 0;

		GameObject* GetOwner() const { return m_pOwner; }
	protected:

		explicit BComponent(GameObject& owner) { m_pOwner = &owner; };

	private:
		GameObject* m_pOwner;
	};
}

