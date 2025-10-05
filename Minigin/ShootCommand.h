#pragma once
#include <Command.h>
#include <glm.hpp>

namespace dae
{
	class Transform;

    class ShootCommand : public CommandObject
    {
    public:
        ShootCommand(GameObject& obj, int idx);
        ~ShootCommand() override = default;

        void Execute() override;
    private:
        int m_PlayerIdx{};

		Transform* m_pTransform{ nullptr };
    };
}

