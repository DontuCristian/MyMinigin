#pragma once
#include "BaseComponent.h"
#include "AIMoveCommand.h"
#include <memory>

namespace dae
{
    class CoilyState;
    class Transform;

    class Coily : public BComponent
    {
    public:
        Coily(GameObject& obj, Transform* playerTransform, bool pvp);
        ~Coily();
        void Update() override;
        void Render() const override;

    private:

        CoilyState* m_pCoilyState;

        Transform* m_pPlayerTransform;
    };
}

