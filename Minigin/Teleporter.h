#pragma once
#include <BaseComponent.h>
#include <glm.hpp>

namespace dae
{
    namespace physics
    {
		struct Rigidbody;
        struct Collider;
        struct CollisionPoints;

    }


    class Teleporter : public BComponent
    {
    public:
            Teleporter(GameObject& obj, const std::vector<glm::ivec2>& destinations);
            ~Teleporter() override = default;

            void Update() override;
			void Render() const override;

    private:


        void OnTrigger(const physics::Collider* other, const physics::CollisionPoints& points);

		std::vector<glm::ivec2> m_Destinations;
    };
}

