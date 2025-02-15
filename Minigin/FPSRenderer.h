#pragma once
#include <memory>
#include "BaseComponent.h"

namespace dae 
{
	class TextRenderer;
	class GameObject;

	class FPSRenderer final : public BComponent
	{
	public:
		FPSRenderer(GameObject* obj);
		~FPSRenderer() = default;

		FPSRenderer(const FPSRenderer& other) = delete;
		FPSRenderer(FPSRenderer&& other) = delete;
		FPSRenderer& operator=(const FPSRenderer& other) = delete;
		FPSRenderer& operator=(FPSRenderer&& other) = delete;

		void Update();
		void Render() const;

		void FixedFPS(bool myBool);

	private:

		TextRenderer* m_TextRenderer;

		bool m_FixedFramerate{true};
	};
}

