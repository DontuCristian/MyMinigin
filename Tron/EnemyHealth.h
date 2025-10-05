#pragma once
#include <BaseComponent.h>
#include "Subject.h"

namespace dae
{
    class EnemyHealth :  public BComponent, public Subject
    {

    public:
        EnemyHealth(GameObject& obj, bool isRecogniser, int maxHealth = 3);

	    ~EnemyHealth() override = default;

        void Update() override {}
        void Render() const override {}

        void TakeDamage();
        void Reset() { m_CurrentHealth = m_MaxHealth; }

        int GetCurrentHealth() const { return m_CurrentHealth; }
	    int GetMaxHealth() const { return m_MaxHealth; }
    private:
            
        int m_MaxHealth;
	    int m_CurrentHealth;

        bool m_IsRecogniser;
    };
}

