#pragma once
#include "BaseComponent.h"
#include "Observer.h"
namespace dae
{
    class TextRenderer;
    class ScoreHUD final : public BComponent, public Observer
    {
    public:

        ScoreHUD(GameObject& obj);
        virtual ~ScoreHUD() = default;

        ScoreHUD(const ScoreHUD& other) = delete;
        ScoreHUD& operator=(const ScoreHUD& other) = delete;
        ScoreHUD(ScoreHUD&& other) noexcept = delete;
        ScoreHUD& operator=(ScoreHUD&& other) noexcept = delete;

        void Update() override;
        void Render() const override;

        void OnNotify(Event event, Subject*) override;

        void SetText(const std::string& text);
        void SetFont(const std::string& fontPath, int size);

    private:

        TextRenderer* m_pTextRenderer;
    };
}

