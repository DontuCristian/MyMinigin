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
		int			RowIdx{};
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
		void SetFrameIdx(int frame) { m_FrameData->CurrentFrame = frame; };
		void SetRowIdx(int row) { m_FrameData->RowIdx = row; };

		int GetFrameWidth() { return m_FrameData->FrameWidth; }
		int GeyFrameHeight() { return m_FrameData->FrameHeight; }

		void PlayAnim() { m_isAnimated = true; };
		void StopAnim() 
		{ 
			m_isAnimated = false; 
			m_FrameData->CurrentFrame = 0;
		};
		void PauseAnim() { m_isAnimated = false; }
	private:

		std::shared_ptr<Texture2D>	m_Texture{};
		SDL_Rect					m_SourceRect{};
		Transform*					m_Transform{};

		std::unique_ptr<FrameData>  m_FrameData{};

		bool m_isAnimated{false};
	};

}

