#pragma once
#include "SDL.h"
#include <memory>
#include <string>
#include "BaseComponent.h"

namespace dae
{
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
		void PlayAnim();
		void StopAnim();
		void ResetAnim();

		void SetFrameDelay(float delay) { m_FrameDelay = delay; };

	private:

		std::shared_ptr<Texture2D> m_Texture{};
		SDL_Rect	m_SourceRect{};
		Transform*  m_Transform{};

		bool		m_IsAnimated{ false };
		int			m_Rows{}, m_Columns{};
		int			m_FrameWidth{ -1 }, m_FrameHeight{ -1 };
		float		m_FrameDelay{};
		float		m_AccumulatedTime{};
		int			m_CurrentFrame{};
		int			m_RowIdx{}, m_ColIdx{};
	};

}

