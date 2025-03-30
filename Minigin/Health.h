#pragma once
#include "BaseComponent.h"
#include "Subject.h"
namespace dae
{
	class GameObject;
	class Health final : public BComponent, public Subject
	{
	public:

		Health(GameObject& obj);
		virtual ~Health() = default;

		Health(const Health& other) = delete;
		Health& operator=(const Health& other) = delete;
		Health(Health&& other) noexcept = delete;
		Health& operator=(Health&& other) noexcept = delete;

		void Update() override;
		void Render() const override;

		void ReduceHealth(int ammmount);
		
	private:
		int m_CurrentHealth{ 100 };
		int m_MaxHealth{ 100 };
	};
}

