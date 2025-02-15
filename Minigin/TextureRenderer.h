#pragma once
#include <memory>
#include <string>
#include "BaseComponent.h"


namespace dae
{
	class Texture2D;
	class Transform;

	class TextureRenderer final : public BComponent
	{
	public:
		TextureRenderer(GameObject* obj);
		~TextureRenderer() = default;

		TextureRenderer(const TextureRenderer&) = delete;
		TextureRenderer(TextureRenderer&&) noexcept = delete;
		TextureRenderer& operator=(const TextureRenderer&) = delete;
		TextureRenderer& operator=(TextureRenderer&&) noexcept = delete;

		void Update() override;
		void FixedUpdate() override;

		void Render() const override;

		void SetTexture( const std::string& path );

	private:

		Transform* m_Transform;
		std::shared_ptr<Texture2D> m_Texture;

	};
}

