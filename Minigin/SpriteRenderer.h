#pragma once
#include "SDL.h"
#include <memory>
#include <string>
#include "BaseComponent.h"

namespace dae
{
	struct FrameData
	{
		int			FrameWidth{ -1 }, FrameHeight{ -1 };
		float		FrameDelay{};
		float		AccumulatedTime{};
		int			CurrentFrame{};
	};

	class GameObject;
	class Texture2D;
	class Transform;

	class SpriteRenderer final : public BComponent
	{
	public:
		virtual ~SpriteRenderer() = default;
		SpriteRenderer(GameObject& owner);

		SpriteRenderer(const SpriteRenderer&) = delete;
		SpriteRenderer(SpriteRenderer&&) noexcept = delete;
		SpriteRenderer& operator=(const SpriteRenderer&) = delete;
		SpriteRenderer& operator=(SpriteRenderer&&) noexcept = delete;

		void Update() override;
		void Render() const override;

		void SetSpriteSheet(const std::string& path, int rows, int columns);

		void SetFrameDelay(float delay) { m_FrameData->FrameDelay = delay; };

	private:

		std::shared_ptr<Texture2D>	m_Texture{};
		SDL_Rect					m_SourceRect{};
		Transform*					m_Transform{};

		std::unique_ptr<FrameData>  m_FrameData{};
	};

}

