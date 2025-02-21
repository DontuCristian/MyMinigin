#pragma once
#include <memory>
#include <sstream>
#include "BaseComponent.h"

namespace dae 
{
	class TextRenderer;
	class GameObject;

	class FPSRenderer final : public BComponent
	{
	public:
		virtual ~FPSRenderer() = default;
		explicit FPSRenderer(GameObject& owner);

		FPSRenderer(const FPSRenderer& other) = delete;
		FPSRenderer(FPSRenderer&& other) = delete;
		FPSRenderer& operator=(const FPSRenderer& other) = delete;
		FPSRenderer& operator=(FPSRenderer&& other) = delete;

		void Update() override;
		void Render() const override;

	private:
		TextRenderer* m_TextRenderer;
	};
}

