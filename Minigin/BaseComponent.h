#pragma once

namespace dae
{
	class GameObject;
	class Transform;

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

		//Helper functions to access components of the owner GameObject thorugh other components
		//template <typename Component>
		//	requires std::derived_from<Component, BComponent>
		//Component* GetOwnerComponent() const
		//{
		//	return m_pOwner->GetComponent<Component>();
		//}
		//
		//template <typename Component>
		//	requires std::derived_from<Component, BComponent>
		//bool HasOwnerComponent() const
		//{
		//	return m_pOwner->HasComponent<Component>();
		//}

		Transform* GetOwnerTransform() const;

	protected:

		explicit BComponent(GameObject& owner) { m_pOwner = &owner; };

	private:
		GameObject* m_pOwner;
	};
}

